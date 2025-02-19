#version 330 core

in flat vec3 FragColor; // No interpolation for flat shading

out vec4 FragOutput;

void main() {
    FragOutput = vec4(FragColor, 1.0);
}
