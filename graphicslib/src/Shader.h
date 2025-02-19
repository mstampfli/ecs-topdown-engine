#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <fstream>
#include <iostream>

class Shader{
    public:
    GLuint shaderProgram;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    ~Shader();

    void use();

    GLint getUniform(const char* uniformName);
    void setUniform(const char* uniformName, int value);

    private:
    const char* readShaderFile(const char* filePath);
    GLuint compileShader(const char* source, GLenum shaderType);
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif