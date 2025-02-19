#include "Shader.h" //First need to import shader because of conflict else
#include "Window.h"
#include "Mesh.h"
#include "Texture.h"
#include <iostream>
#include <vector>


Window window; //If uses constructor need to call window()
std::vector<Mesh*> drawObjects;

float i = 1;
void update(){
    // Update game state here
    std::cout << "Hello" << std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    (*drawObjects[0]).draw();
    float* pos = window.toOpenGLCoordinates(200.0f + 200.0f + 20.0f * i,200.0f + 200.0f);
    (*drawObjects[0]).x = pos[0];
    (*drawObjects[0]).y = pos[1];
    delete[] pos;

    (*drawObjects[0]).updateModelMatrix();
    (*drawObjects[0]).loadModelMatrix();
    i++;
}


int main() {
    std::cout << "Entered Main" << std::endl;

    window.open(800, 600, "Window");


    window.initializeOpenGL();
    window.setFramerate(1);
    glClearColor(0.3f, 0.5f, 0.1f, 1.0f);
    window.updateFunction = update;

    std::cout << "Initialised Window" << std::endl;

    Shader shader("./shaders/texture/vertex.glsl", "./shaders/texture/fragment.glsl");
    Texture myTexture("./assets/boss.jpg");

    std::cout << "Loaded Shader and texture" << std::endl;

    shader.use();
    myTexture.bind(0); // Bind to texture unit 0
    glUniform1i(glGetUniformLocation(shader.shaderProgram, "tex"), 0); // Tell shader to use texture unit 0

    std::cout << "Bound texture" << std::endl;

    Mesh* rectangle = createRectangle();
    drawObjects.push_back(rectangle);
    (*rectangle).getUniforms(shader.shaderProgram);
    float* pos = window.toOpenGLCoordinates(200.0f + 200.0f,200.0f + 200.0f); //Pos + size / 2 since origin is centered
    (*rectangle).x = pos[0];
    (*rectangle).y = pos[1];
    delete[] pos;
    float* size = window.toOpenGLSize(400.0f,400.0f);
    (*rectangle).w = size[0];
    (*rectangle).h = size[1];
    delete[] size;
    (*rectangle).updateModelMatrix();
    (*rectangle).loadModelMatrix();

    std::cout << "Created Mesh" << std::endl;

    window.run();

    std::cin.get();

    window.close();
    delete rectangle;
    return 0;
}
