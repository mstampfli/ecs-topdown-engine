/*#ifndef MESH_H
#define MESH_H

#include "Window.h"
#include "../../glad/include/glad.h"
#include "../../glfw/include/glfw3.h"
#include <vector>
#include <iostream>
#include "../../glm/mat4x4.hpp"  // For 4x4 matrices (e.g., glm::mat4)

float* createMatrix();

class Mesh {
public:
    float x = 0,y = 0,z = 0;
    float w = 1,h = 1, l = 1;

    Window* windowReference;

    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, Window* window, GLuint shaderProgram);
    ~Mesh();
    
    void getUniforms(GLuint shaderProgram);
    void updateModelMatrix();
    void loadModelMatrix();
    void draw() const;

    void setPixelPosition(int px, int py);
    void setPixelSize(int pw, int ph);
    void setColor(float r, float g, float b, float a);

private:
    GLuint VAO, VBO, EBO;
    size_t indexCount;


    float* modelMatrix = createMatrix();
    GLint colorUniform;
    GLint modelUniform;
    //GLuint viewUniform;
    //GLuint projectionUniform;
};

Mesh* createRectangle(Window* window, GLuint shaderProgram);


#endif*/