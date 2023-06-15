#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "BaseLight.hpp"

class DirectLight : public BaseLight
{
public:
    DirectLight()
    : m_direction{0.0f}
    {}

    void render(MyGL::Shader &shader, std::string prefix = "directLight") override
    {
        using std::string;
        glUniform3fv(shader.getLoc(string(prefix + ".dir")), 1, glm::value_ptr(m_direction));
        glUniform3fv(shader.getLoc(string(prefix + ".diffuse")), 1, glm::value_ptr(m_diffuse));
        glUniform3fv(shader.getLoc(string(prefix + ".ambient")), 1, glm::value_ptr(m_ambient));
        glUniform3fv(shader.getLoc(string(prefix + ".specular")), 1, glm::value_ptr(m_specular));
    };

    void setDirection(glm::vec3 value) { m_direction = value; }
    const glm::vec3& getDirection() { return m_direction; }

private:
    glm::vec3 m_direction;
};