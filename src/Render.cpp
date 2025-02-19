#include <unordered_map>
#include "headers/Render.h"
#include "headers/Components.h"
#include "headers/Entity.h"
#include "headers/DeltaTime.h"
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
extern DeltaTime& dt;

void initRendering() {
    
    rm.window.open(1200, 800, "Window");

    rm.window.initializeOpenGL();
    rm.shaders["default"] = std::make_shared<Shader>("../graphicslib/shaders/texture/vertex.glsl", "../graphicslib/shaders/texture/fragment.glsl");

    rm.window.setFramerate(120);

    glClearColor(0.3f, 0.5f, 0.1f, 1.0f);
    
    std::cout << "Initialised Window" << std::endl;

    rm.textures["player"] = std::make_shared<Texture>("../assets/textures/person.jpg");
    rm.textures["default"] = std::make_shared<Texture>("../assets/textures/Download.jpg");

    std::cout << "Loaded Shader and texture" << std::endl;
}

void updateEntityRectangles() {
    for (Entity entity : rm.entityManager.getEntities()) {
        // If mesh does not exist, create it
        if (rm.meshComponents.find(entity) == rm.meshComponents.end()) {
            rm.meshComponents[entity] = std::make_unique<Mesh>(*createRectangle());
        }

        Mesh* mesh = rm.meshComponents[entity].get();  // Get the existing mesh

        // Update shader uniforms
        if (rm.shaderComponents.find(entity) != rm.shaderComponents.end()) {
            (*mesh).getUniforms((*rm.shaderComponents[entity]).shaderProgram);
        } else {
            (*mesh).getUniforms((*rm.shaders["default"]).shaderProgram);
        }

        // Update mesh position & size
        float* pos = rm.window.toOpenGLCoordinates(
            (*rm.positionComponents[entity]).x + (*rm.sizeComponents[entity]).w / 2, 
            (*rm.positionComponents[entity]).y + (*rm.sizeComponents[entity]).h / 2
        );
        (*mesh).x = pos[0];
        (*mesh).y = pos[1];
        delete[] pos;

        float* size = rm.window.toOpenGLSize(
            (*rm.sizeComponents[entity]).w, 
            (*rm.sizeComponents[entity]).h
        );

        (*mesh).w = size[0];
        (*mesh).h = size[1];
        delete[] size;

        // Update transformation matrices
        (*mesh).updateModelMatrix();
        (*mesh).loadModelMatrix();
    }
}

void renderPlayers() {

    updateEntityRectangles();
    glClear(GL_COLOR_BUFFER_BIT);
    sortEntitiesByYValue();

    float deltaTime = dt.getDeltaTime();

    for (auto& entity : rm.sortedDrawEntities) {
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

        (*rm.meshComponents[entity]).updateModelMatrix();
        (*rm.meshComponents[entity]).loadModelMatrix();
        (*rm.meshComponents[entity]).draw();
        (*rm.behaviours[entity]).update(entity, deltaTime);
    }
    rm.eventSystem.update(deltaTime);
    rm.movementSystem.update(deltaTime);
    rm.combatSystem.update(deltaTime);
}

void sortEntitiesByYValue() {
    rm.sortedDrawEntities.clear();

    for (auto& entity : rm.entityManager.getEntities()) {
        rm.sortedDrawEntities.push_back(entity);
    }
    std::stable_sort(rm.sortedDrawEntities.begin(), rm.sortedDrawEntities.end(), 
        [](Entity a, Entity b) {
            return (*rm.positionComponents[a]).y < (*rm.positionComponents[b]).y;
        }
    );
}