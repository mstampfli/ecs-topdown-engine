#ifndef SHADER_H
#define SHADER_H

#include "../../glad/include/glad/glad.h"
#include "../../glfw/include/GLFW/glfw3.h"
#include <fstream>
#include <iostream>

class Shader{
    public:
    GLuint shaderProgram;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    ~Shader();

    void use();

    GLint getUniform(const char* uniformName);
    void setUniform(GLint uniform, int value);

    void setTexture(int slot);

    private:
    const char* readShaderFile(const char* filePath);
    GLuint compileShader(const char* source, GLenum shaderType);
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
    GLint textureUniform;
    GLint useTextureUniform;
};

#endif