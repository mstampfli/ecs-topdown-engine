#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture {
public:
    // Load a texture from file (expects a PNG with transparency).
    Texture(const char* filePath);
    ~Texture();

    // Bind the texture to a specified texture slot.
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    GLuint textureID; // Exposed so the renderer can refer to it if needed.
private:
    int width, height, channels;
};

#endif
