#include "Renderer.h"
#include <glad/glad.h>
#include "../../glm/gtc/matrix_transform.hpp"
#include <iostream>

// Vertex data for a unit quad.
static float vertices[] = {
    // positions         // texture coordinates
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
};

static unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

Renderer::Renderer(Window* window, const char* vertexShaderPath, const char* fragmentShaderPath)
    : windowHandle(window), shader(nullptr), MAX_INSTANCES(1000), vertexCount(6),
      currentVertexShaderPath(vertexShaderPath), currentFragmentShaderPath(fragmentShaderPath)
{
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &elementEBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteVertexArrays(1, &vao);
    if(shader) {
        delete shader;
    }
}

void Renderer::initialize() {
    // Initialize texture slots (max 16).
    textureSlots.resize(16, nullptr);

    // Create and bind the VAO.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Set up vertex buffer.
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up element buffer.
    glGenBuffers(1, &elementEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes:
    // Position attribute (location = 0).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coordinate attribute (location = 1).
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set up instance VBO for per-instance data.
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(RenderInstanceData), nullptr, GL_DYNAMIC_DRAW);

    // Instance attributes:
    // The model matrix (mat4) occupies locations 2-5.
    for (int i = 0; i < 4; i++) {
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(RenderInstanceData),
                              (void*)(i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }

    // Color attribute (location = 6).
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(RenderInstanceData),
                          (void*)(sizeof(glm::mat4)));
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);

    // TextureID attribute (location = 7).
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(RenderInstanceData),
                          (void*)(sizeof(glm::mat4) + sizeof(glm::vec4)));
    glEnableVertexAttribArray(7);
    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);

    // Create and configure shader.
    shader = new Shader(currentVertexShaderPath.c_str(), currentFragmentShaderPath.c_str());
    shader->use();
    // Set up an orthographic projection based on the window dimensions.
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowHandle->getWidth()),
                                      static_cast<float>(windowHandle->getHeight()), 0.0f,
                                      -1.0f, 1.0f);
    glUniformMatrix4fv(shader->getUniform("projection"), 1, GL_FALSE, &projection[0][0]);

    // Bind texture sampler array (supports up to 16 texture units).
    int samplers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    glUniform1iv(shader->getUniform("textures"), 16, samplers);
}

void Renderer::beginDraw() {
    instanceData.clear();
}

void Renderer::endDraw() {
    if (instanceData.empty()) return;

    // Update instance buffer data.
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceData.size() * sizeof(RenderInstanceData), instanceData.data());

    // Bind all textures stored in textureSlots.
    for (uint32_t i = 0; i < textureSlots.size(); i++) {
        if (textureSlots[i] != nullptr) {
            textureSlots[i]->bind(i);
        }
    }

    // Render all instances in one draw call.
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0, instanceData.size());
    glBindVertexArray(0);

    // Reset texture slots for next frame.
    std::fill(textureSlots.begin(), textureSlots.end(), nullptr);
}

void Renderer::drawRectangle(const glm::vec3& position,
                             const glm::vec2& scale,
                             float rotation,
                             const glm::vec4& color,
                             uint32_t textureID) {
    if (instanceData.size() >= static_cast<size_t>(MAX_INSTANCES))
        return;

    RenderInstanceData instance;
    glm::mat4 model = glm::mat4(1.0f);
    // Translate to the specified position (offset by half scale to center the quad).
    model = glm::translate(model, position + glm::vec3(scale.x * 0.5f, scale.y * 0.5f, 0.0f));
    // Rotate around the Z axis.
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    // Scale the quad.
    model = glm::scale(model, glm::vec3(scale, 1.0f));

    instance.model = model;
    instance.color = color;
    instance.textureID = static_cast<float>(textureID);
    instance.pad = glm::vec3(0.0f);

    instanceData.push_back(instance);
}

void Renderer::drawTextureEx(Texture* texture,
                             const glm::vec3& position,
                             const glm::vec2& scale,
                             float rotation,
                             const glm::vec4& tint) {
    if (!texture) return;
    // Obtain a texture slot index for this texture.
    uint32_t slot = getTextureSlot(texture);
    // Bind the texture to its slot.
    texture->bind(slot);
    // Queue the draw call.
    drawRectangle(position, scale, rotation, tint, slot);
}

uint32_t Renderer::getTextureSlot(Texture* texture) {
    // Check if texture is already registered.
    for (uint32_t i = 0; i < textureSlots.size(); i++) {
        if (textureSlots[i] == texture) {
            return i;
        }
    }
    // Register in the first available slot.
    for (uint32_t i = 0; i < textureSlots.size(); i++) {
        if (textureSlots[i] == nullptr) {
            textureSlots[i] = texture;
            return i;
        }
    }
    // If all slots are in use, default to slot 0.
    return 0;
}

void Renderer::setShader(const char* vertexShaderPath, const char* fragmentShaderPath) {
    // Delete the old shader if it exists.
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    // Update stored paths.
    currentVertexShaderPath = vertexShaderPath;
    currentFragmentShaderPath = fragmentShaderPath;
    // Create the new shader.
    shader = new Shader(currentVertexShaderPath.c_str(), currentFragmentShaderPath.c_str());
    shader->use();
    // Re-set uniforms (projection and texture sampler array).
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowHandle->getWidth()),
                                      static_cast<float>(windowHandle->getHeight()), 0.0f,
                                      -1.0f, 1.0f);
    glUniformMatrix4fv(shader->getUniform("projection"), 1, GL_FALSE, &projection[0][0]);
    int samplers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    glUniform1iv(shader->getUniform("textures"), 16, samplers);
}
