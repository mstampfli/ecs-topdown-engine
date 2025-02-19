#version 330 core

layout(location = 0) in vec3 aPos;     // Vertex position
layout(location = 1) in vec3 aNormal;  // Vertex normal

out flat vec3 FragColor; // Flat shading means no interpolation

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightDir;  // Directional light direction
uniform vec3 color;     // Object color

void main() {
    // Transform the normal to world space
    vec3 normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    // Compute simple diffuse lighting
    float diff = max(dot(normal, normalize(lightDir)), 0.0);

    // Compute final color
    FragColor = diff * color;

    // Transform vertex position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
