#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION  // This must be defined before including stb_image.h
#include <stb/stb_image.h>

#include <iostream>
#include <glad/glad.h>


Texture::Texture(const char* filePath){
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(filePath, &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return;
    }

    // Generate and bind texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters (important!)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free image data (no longer needed after uploading to GPU)
    stbi_image_free(data);
}
Texture::~Texture() {
    glDeleteTextures(1, &textureID);
}
void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}