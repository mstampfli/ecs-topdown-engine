#include <unordered_map>
#include "headers/Render.h"
#include "headers/Components.h"
#include "headers/Entity.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "../graphicslib/src/Window.h" 
#include "../graphicslib/src/Shader.h" 
#include "../graphicslib/src/Mesh.h" 
#include "../graphicslib/src/Texture.h"
#include "headers/ResourceManager.h"
#include "headers/Systems.h"

extern ResourceManager& rm;

void initRendering() {
    
    rm.window.open(800, 600, "Window");

    rm.window.initializeOpenGL();
    rm.shader = std::make_unique<Shader>("../graphicslib/shaders/texture/vertex.glsl", "../graphicslib/shaders/texture/fragment.glsl");

    rm.window.setFramerate(60);
    glClearColor(0.3f, 0.5f, 0.1f, 1.0f);
    
    std::cout << "Initialised Window" << std::endl;

    rm.texture = std::make_unique<Texture>("../assets/textures/person.jpg");

    std::cout << "Loaded Shader and texture" << std::endl;

    (*rm.shader).use();
    (*rm.texture).bind(0); // Bind to texture unit 0
}

void initEntityRectangles() {
    for (Entity entity : rm.entityManager.getEntities()) {
        Mesh* rectangle = createRectangle();
        rectangle = createRectangle();
        rm.drawObjects.push_back(rectangle);
        (*rectangle).getUniforms((*rm.shader).shaderProgram);
        float* pos = rm.window.toOpenGLCoordinates(rm.positionComponents[entity].x + rm.sizeComponents[entity].w / 2, rm.positionComponents[entity].y + rm.sizeComponents[entity].h / 2); //Pos + size / 2 since origin is centered
        (*rectangle).x = pos[0];
        (*rectangle).y = pos[1];
        delete[] pos;
        float* size = rm.window.toOpenGLSize(rm.sizeComponents[entity].w, rm.sizeComponents[entity].h);
        (*rectangle).w = size[0];
        (*rectangle).h = size[1];
        delete[] size;
        (*rectangle).updateModelMatrix();
        (*rectangle).loadModelMatrix();
    }  
}

void renderPlayers() {
    rm.drawObjects.clear();
    initEntityRectangles();

    glClear(GL_COLOR_BUFFER_BIT);
    (*rm.shader).use();
    (*rm.texture).bind(0);

    glUniform1i(glGetUniformLocation((*rm.shader).shaderProgram, "tex"), 0); // Tell shader to use texture unit 0
    (*rm.drawObjects[0]).draw();
    
    for (auto& [entity, behaviour] : rm.behaviours) {
        behaviour->update(entity, 1.0f);
    }

    updateMovement(1.0f);
}
