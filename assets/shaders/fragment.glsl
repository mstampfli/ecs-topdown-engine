#version 430 core

in vec2 uv;
in vec4 instanceColor;
flat in uint instanceTexture;

uniform sampler2D tex[16]; // Assume max 16 textures

out vec4 fragmentColor;

void main() {
    vec4 texColor = texture(tex[instanceTexture], uv);
    fragmentColor = instanceColor * texColor;
}
