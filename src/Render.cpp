#include <unordered_map>
#include "headers/Render.h"
#include "headers/Components.h"
#include "headers/Entity.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "../graphicslib/src/Window.h" 
#include "../graphicslib/src/Shader.h" 
#include "../graphicslib/src/Mesh.h" 
#include "../graphicslib/src/Texture.h"
#include "headers/ResourceManager.h"
#include "headers/Systems.h"

extern ResourceManager& rm;

std::vector<Entity> sortEntitiesByYValue();
void initRendering() {
    
    rm.window.open(800, 600, "Window");

    rm.window.initializeOpenGL();
    rm.shaders["default"] = std::make_shared<Shader>("../graphicslib/shaders/texture/vertex.glsl", "../graphicslib/shaders/texture/fragment.glsl");

    rm.window.setFramerate(60);
    glClearColor(0.3f, 0.5f, 0.1f, 1.0f);
    
    std::cout << "Initialised Window" << std::endl;

    rm.textures["player"] = std::make_shared<Texture>("../assets/textures/person.jpg");

    std::cout << "Loaded Shader and texture" << std::endl;
}

void initEntityRectangles() {
    for (Entity entity : rm.entityManager.getEntities()) {
        Mesh* mesh = createRectangle();
        
        rm.meshComponents[entity] = std::make_unique<Mesh>(*mesh);

        if (rm.shaderComponents.find(entity) != rm.shaderComponents.end()) {
            (*mesh).getUniforms((*rm.shaderComponents[entity]).shaderProgram);
        } else {
            (*mesh).getUniforms((*rm.shaders["default"]).shaderProgram);
        }

        float* pos = rm.window.toOpenGLCoordinates(rm.positionComponents[entity].x + rm.sizeComponents[entity].w / 2, rm.positionComponents[entity].y + rm.sizeComponents[entity].h / 2); //Pos + size / 2 since origin is centered
        (*mesh).x = pos[0];
        (*mesh).y = pos[1];
        delete[] pos;
        float* size = rm.window.toOpenGLSize(rm.sizeComponents[entity].w, rm.sizeComponents[entity].h);
        (*mesh).w = size[0];
        (*mesh).h = size[1];
        delete[] size;
        (*mesh).updateModelMatrix();
        (*mesh).loadModelMatrix();

        rm.drawObjects.push_back(mesh);
    }  
}

void renderPlayers() {
    rm.drawObjects.clear();
    initEntityRectangles();

    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& entity : sortEntitiesByYValue()) {
        if (rm.textureComponents.find(entity) != rm.textureComponents.end()) {
            (*rm.textureComponents[entity]).bind(0);
        } else {
            (*rm.textures["default"]).bind(0);
        }

        if (rm.shaderComponents.find(entity) != rm.shaderComponents.end()) {
            (*rm.shaderComponents[entity]).use();
            glUniform1i(glGetUniformLocation((*rm.shaderComponents[entity]).shaderProgram, "tex"), 0);
        } else {
            (*rm.shaders["default"]).use();
            glUniform1i(glGetUniformLocation((*rm.shaders["default"]).shaderProgram, "tex"), 0);
        }

        (*rm.behaviours[entity]).update(entity, 1.0f);
        (*rm.meshComponents[entity]).draw();
    }

    updateMovement(1.0f);
}

std::vector<Entity> sortEntitiesByYValue() {
    std::vector<Entity> sortedEntities;

    for (const auto& entity : rm.entityManager.getEntities()) {
        sortedEntities.push_back(entity);
    }
    
    // Sort entities from highest `y` to lowest
    std::sort(sortedEntities.begin(), sortedEntities.end(), 
        [](Entity a, Entity b) {
            return rm.positionComponents[a].y > rm.positionComponents[b].y;  
        }
    );

    return sortedEntities;
}