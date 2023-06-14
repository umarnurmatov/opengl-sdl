#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Base/Shader.hpp"
#include <string>

class BaseLight
{
public:
    BaseLight()
    :   m_color{1.0f},
        m_ambient{0.2f},
        m_diffuse{0.5f},
        m_specular{1.0f}
    {}

    virtual void render(MyGL::Shader &shader, std::string prefix) = 0;

    void setDiffuseColor(glm::vec3 value) { m_color = value; m_ambient = 0.2f * value; m_diffuse = 0.5f * value; }
    void setSpecularColor(glm::vec3 value) { m_specular = value; }

    glm::vec3& getColor() { return m_color; }

protected:
    glm::vec3 m_color;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
};