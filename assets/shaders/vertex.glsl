#version 430 core

layout(location = 0) in vec3 inPosition;  // Mesh vertex position
layout(location = 1) in vec2 inUV;        // Mesh UV

struct RenderMetaData {
    vec3 position;
    float padding1;
    vec3 scale;
    float padding2;
    vec4 color;
    uint texture;
    float padding3[3];
};

layout(std430, binding = 0) buffer RenderBuffer {
    RenderMetaData meta[];
};

out vec2 uv;
out vec4 instanceColor;
flat out uint instanceTexture;

void main() {
    int id = gl_InstanceID; // Instance index
    vec3 worldPos = inPosition * meta[id].scale + meta[id].position; // Scale & move object

    gl_Position = vec4(worldPos, 1.0);
    uv = inUV;
    instanceColor = meta[id].color;
    instanceTexture = meta[id].texture;
}
