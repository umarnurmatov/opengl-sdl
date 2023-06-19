#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "Shader.hpp"
#include "Log.hpp"


namespace Engine
{

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoords;
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
    std::vector<Texture*> textures;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture*>& textures);
    void draw(Shader &shader) const;

private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};

};