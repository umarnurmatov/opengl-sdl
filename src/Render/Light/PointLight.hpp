#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "BaseLight.hpp"

class PointLight : public BaseLight
{
public:
    PointLight()
    : m_constant{1.0f},
      m_linear{0.35f},
      m_quadratic{0.44f},
      m_position{1.0f}
    {}

    void render(MyGL::Shader &shader, std::string prefix) override
    {
        using std::string;
        glUniform3fv(shader.getLoc(string(prefix + ".pos")), 1, glm::value_ptr(m_position));
        glUniform3fv(shader.getLoc(string(prefix + ".diffuse")), 1, glm::value_ptr(m_diffuse));
        glUniform3fv(shader.getLoc(string(prefix + ".ambient")), 1, glm::value_ptr(m_ambient));
        glUniform3fv(shader.getLoc(string(prefix + ".specular")), 1, glm::value_ptr(m_specular));

        glUniform1f(shader.getLoc(string(prefix + ".constant")), m_constant);
        glUniform1f(shader.getLoc(string(prefix + ".linear")), m_linear);
        glUniform1f(shader.getLoc(string(prefix + ".quadratic")), m_quadratic);
    };

    void setPosition(glm::vec3 value) { m_position = value; }
    const glm::vec3& getPosition() { return m_position; }

private:
    glm::vec3 m_position;

    float m_constant;
    float m_linear;
    float m_quadratic;
};