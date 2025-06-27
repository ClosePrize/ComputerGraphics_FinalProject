#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Texture.h"
#include "Shader.h"
#include "Vertex.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;  // Texture eklendi
    glm::vec3 diffuseColor;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec3 color);
    void Draw(Shader& shader);

private:
    unsigned int VBO, EBO;
    void setupMesh();
};

#endif
