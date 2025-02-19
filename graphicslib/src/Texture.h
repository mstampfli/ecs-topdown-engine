#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture{
    public:
    GLuint textureID;
    GLuint64 handle;


    Texture(const char* filePath);
    ~Texture();
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    private:
    unsigned char* data;
    int width;
    int height;
    int channels;
};

#endif