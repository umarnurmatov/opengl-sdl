#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "Shader.hpp"

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 texCoords;
};

struct Texture
{
    GLuint id;
    enum Type { SPECULAR, DIFFUSE } type;
};
    
class Mesh    
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
    void draw(MyGL::Shader &shader) const;

private:
    GLuint m_VAO, m_VBO, m_EBO;
    void m_setupMesh();
};