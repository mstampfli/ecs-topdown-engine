#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 acolor;
uniform mat4 modelMatrix;

out vec3 color;

void main() {
    gl_Position = modelMatrix * vec4(position, 1.0);
    color = acolor;
}
