#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

unsigned int TextureFromFile(const char* path, const std::string& directory);

#endif
