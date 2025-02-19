#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath){
    const char* vertexShaderSource = readShaderFile(vertexShaderPath);
    const char* fragmentShaderSource = readShaderFile(fragmentShaderPath);

    GLuint vertexShaderProgram = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShaderProgram = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    shaderProgram = linkProgram(vertexShaderProgram, fragmentShaderProgram);
}

Shader::~Shader(){
    glDeleteProgram(shaderProgram);
}

void Shader::use(){
    glUseProgram(shaderProgram);
}

GLint Shader::getUniform(const char* uniformName){
    return glGetUniformLocation(shaderProgram, uniformName);
}

//Need multiple versions for different types of uniforms
void Shader::setUniform(const char* uniformName, int value){
    glUniform1i(glGetUniformLocation(shaderProgram, uniformName), value);
}

const char* Shader::readShaderFile(const char* filePath){
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate); // Open file in binary mode and move to the end
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return nullptr;
    }

    std::streamsize fileSize = file.tellg(); // Get the file size
    file.seekg(0, std::ios::beg); // Move back to the beginning of the file

    // Allocate buffer to hold the content
    char* buffer = new char[fileSize + 1]; // +1 for null terminator
    if (!file.read(buffer, fileSize)) {
        std::cerr << "Failed to read shader file: " << filePath << std::endl;
        delete[] buffer; // Clean up on failure
        return nullptr;
    }

    buffer[fileSize] = '\0'; // Null-terminate the string
    file.close(); // Close the file

    return buffer;
}

GLuint Shader::compileShader(const char* source, GLenum shaderType){
    GLuint shader = glCreateShader(shaderType);

    if (shader == 0) {
        std::cerr << "Failed to create shader of type " 
                  << (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
        return 0;
    }

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Failed (" 
                  << (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment") 
                  << ")\n" << infoLog << std::endl;
        glDeleteShader(shader);  
        return 0;
    }


    delete[] source;

    return shader;
}

GLuint Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader){
    if (vertexShader == 0 || fragmentShader == 0) {
        std::cerr << "Shader linking failed: One or more shaders are invalid." << std::endl;
        return 0;
    }
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}