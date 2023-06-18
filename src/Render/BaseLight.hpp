#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.hpp"
#include <string>

namespace Engine
{

class BaseLight
{
public:
    BaseLight()
    :   color{1.0f},
        ambient{0.2f},
        diffuse{0.5f},
        specular{1.0f}
    {}

    virtual void render(Shader &shader, std::string prefix) = 0;

    void setDiffuseColor(glm::vec3 value) { color = value; ambient = 0.2f * value; diffuse = 0.5f * value; }
    void setSpecularColor(glm::vec3 value) { specular = value; }

    glm::vec3& getColor() { return color; }

protected:
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

};