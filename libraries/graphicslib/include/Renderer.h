#ifndef RENDERER_H
#define RENDERER_H

#include "../../glad/include/glad/glad.h"
#include <vector>
#include "Window.h"

struct vec2 {
    float x, y;
};

struct vec3 {
    float x, y, z;
};

struct vec4 {
    float r, g, b, a;
};


struct RenderMetaData {
    vec3 position;  // Position (X, Y, Z)
    float padding1;      // Padding for 16-byte alignment
    vec3 scale;     // Scale (Width, Height, Depth)
    float padding2;
    vec4 color;     // RGBA color
    uint32_t texture;    // Texture slot or handle
    float padding3[3];   // Extra padding for alignment
};


class Renderer{
    public:
    Renderer(Window* window);
    ~Renderer();

    void initialize();

    void beginDraw();
    void endDraw();

    void drawRectangle(vec3 position, vec3 scale, vec4 color, uint32_t textureID);

    protected:
    Window* windowHandle;

    GLuint ssbo;
    GLuint vao;
    int MAX_INSTANCES;
    int vertexCount;
    std::vector<RenderMetaData> instanceData; // Fill this with data for each instance
};

void toOpenGLColor(float& r, float& g, float& b, float& a);

#endif
