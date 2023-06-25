#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;

    VAO VAO;

    Mesh(std::vector <Vertex>& vertices,
         std::vector <GLuint>& indices,
         std::vector <Texture>& textures);

    // Draws the mesh
    void Draw
            (
                Shader& shader,
                Camera& camera,
                glm::vec3 translation,
                glm::mat4 matrix = glm::mat4(1.0f),
                glm::quat rotation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f),
                glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
            );
};
#endif