#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include "../../glm/glm.hpp"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"

// Data for each instance (textured quad) to be drawn.
struct RenderInstanceData {
    glm::mat4 model;   // Model matrix (translation, rotation, scale)
    glm::vec4 color;   // Color tint (RGBA)
    float textureID;   // Texture slot index (stored as float for the shader)
    glm::vec3 pad;     // Padding for alignment
};

class Renderer {
public:
    // Attach the renderer to a window, using the given shader paths (defaults provided).
    Renderer(Window* window, const char* vertexShaderPath = "texture.vert", const char* fragmentShaderPath = "texture.frag");
    virtual ~Renderer();

    // Initialize VAO/VBOs, shader, and instance buffer.
    void initialize();

    // Clear any queued instances.
    void beginDraw();

    // Upload instance data, bind textures, and perform the instanced draw call.
    void endDraw();

    // Low-level draw function: queue a textured quad.
    void drawRectangle(const glm::vec3& position,
                       const glm::vec2& scale,
                       float rotation,
                       const glm::vec4& color,
                       uint32_t textureID);

    // High-level draw function: queue a textured quad using a Texture pointer.
    void drawTextureEx(Texture* texture,
                       const glm::vec3& position,
                       const glm::vec2& scale,
                       float rotation,
                       const glm::vec4& tint);

    // Change the shader by providing new vertex and fragment shader paths.
    void setShader(const char* vertexShaderPath, const char* fragmentShaderPath);

protected:
    // Helper to get a texture slot index for a given texture.
    uint32_t getTextureSlot(Texture* texture);

    Window* windowHandle;
    Shader* shader; // Shader used for rendering.
    
    GLuint vao;         // Vertex Array Object.
    GLuint vertexVBO;   // VBO for quad vertex data.
    GLuint elementEBO;  // Element buffer for quad indices.
    GLuint instanceVBO; // VBO for per-instance data.

    int MAX_INSTANCES;  // Maximum instances per frame.
    int vertexCount;    // Number of indices to draw per quad.

    // Collected instance data.
    std::vector<RenderInstanceData> instanceData;

    // Texture slots mapping (max 16 textures).
    std::vector<Texture*> textureSlots;

    // Store the current shader paths.
    std::string currentVertexShaderPath;
    std::string currentFragmentShaderPath;
};

#endif
