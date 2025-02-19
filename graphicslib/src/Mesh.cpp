#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    // Set uv attribute pointers
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // TexCoords
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

    // Store the number of indices for drawing
    indexCount = indices.size();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::getUniforms(GLuint shaderProgram) {
    modelUniform = glGetUniformLocation(shaderProgram, "model");
}

void Mesh::updateModelMatrix() {
    modelMatrix[12] = x;
    modelMatrix[13] = y;
    modelMatrix[14] = z;

    modelMatrix[0] = w;
    modelMatrix[5] = h;
    modelMatrix[10] = l;
}
    
void Mesh::loadModelMatrix() {
    glUniformMatrix4fv(modelUniform, 1, 0, &modelMatrix[0]);
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh* createRectangle() {
    // Vertex data for a rectangle (positions & uv)
    std::vector<float> vertices = {
        // x     y     z           u     v
        -0.5f, -0.5f, 0.0f,       0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, 0.0f,       1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f, 0.0f,       1.0f, 1.0f, // Top-right
        -0.5f,  0.5f, 0.0f,       0.0f, 1.0f  // Top-left
    };

    // Index data for two triangles
    std::vector<unsigned int> indices = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };
    Mesh* rectangle = new Mesh(vertices, indices);
    return rectangle;
}

float* createMatrix(){
    float* matrix = new float[16];
    for(int i = 0; i < 16; ++i){
        matrix[i] = 0.0f;
    }
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;

    return matrix;
}

