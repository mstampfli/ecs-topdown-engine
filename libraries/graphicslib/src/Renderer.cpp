#include "Renderer.h"

Renderer::Renderer(Window* window){
    MAX_INSTANCES = 1000; //Maximum amount of renderable Instances
    vertexCount = 6; //Rectangle

    windowHandle = window;
}

Renderer::~Renderer(){
    glDeleteBuffers(1, &ssbo);
    glDeleteVertexArrays(1, &vao);
}

void Renderer::initialize(){
    std::vector<float> vertices = {
        // x     y     z        u     v
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // Top-right
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // Top-left
    };

    // Index data for two triangles
    std::vector<unsigned int> indices = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Generate EBO
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Define vertex attribute pointers
    // Position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

    // Setup SSBO for instancing
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_INSTANCES * sizeof(RenderMetaData), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
}

void Renderer::beginDraw(){
    instanceData.clear();
}

void Renderer::endDraw(){
    if (instanceData.empty()) return;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, instanceData.size() * sizeof(RenderMetaData), instanceData.data());

    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0, instanceData.size());
}

void Renderer::drawRectangle(vec3 position, vec3 scale, vec4 color, uint32_t textureID) {
    if (instanceData.size() >= MAX_INSTANCES) return;

    RenderMetaData instance;
    position.x += scale.x / 2.0f;
    position.y += scale.y / 2.0f;
    windowHandle->toOpenGLCoordinates(position.x, position.y);
    windowHandle->toOpenGLSize(scale.x, scale.y);
    toOpenGLColor(color.r, color.g, color.b, color.a);

    instance.position = position;
    instance.scale = scale;
    instance.color = color;
    instance.texture = textureID;
    

    instanceData.push_back(instance); // Accumulate instance
}

void toOpenGLColor(float& r, float& g, float& b, float& a){
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;
    a /= 255.0f;
}
