#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <glm/mat4x4.hpp>  // For 4x4 matrices (e.g., glm::mat4)

float* createMatrix();

class Mesh {
public:
    float x = 0,y = 0,z = 0;
    float w = 1,h = 1, l = 1;

    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();
    
    void getUniforms(GLuint shaderProgram);
    void updateModelMatrix();
    void loadModelMatrix();
    void draw() const;

private:
    GLuint VAO, VBO, EBO;
    size_t indexCount;


    float* modelMatrix = createMatrix();
    GLuint modelUniform;
    //GLuint viewUniform;
    //GLuint projectionUniform;
};

Mesh* createRectangle();


#endif