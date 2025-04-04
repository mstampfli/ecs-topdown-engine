#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
public:
    // Create a shader program from vertex and fragment shader file paths.
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    // Activate the shader program.
    void use();
    // Get the location of a uniform variable.
    GLint getUniform(const char* uniformName);

    GLuint shaderProgram; // Exposed for simplicity.
private:
    std::string readShaderFile(const char* filePath);
    GLuint compileShader(const char* source, GLenum shaderType);
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif
