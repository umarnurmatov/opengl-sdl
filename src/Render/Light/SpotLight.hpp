#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "BaseLight.hpp"

class SpotLight : public BaseLight
{
public:
    SpotLight()
    : m_constant{1.0f},
      m_linear{0.35f},
      m_quadratic{0.44f},

      m_position{1.0f},
      m_direction{1.0f},
      
      m_cutOffIn{25.0f},
      m_cutOffOut{35.0f}
    {}

    void render(MyGL::Shader &shader, std::string prefix) override
    {
        using std::string;
        glUniform3fv(shader.getLoc(string(prefix + ".pos")), 1, glm::value_ptr(m_position));
        glUniform3fv(shader.getLoc(string(prefix + ".dir")), 1, glm::value_ptr(m_direction));

        glUniform3fv(shader.getLoc(string(prefix + ".diffuse")), 1, glm::value_ptr(m_diffuse));
        glUniform3fv(shader.getLoc(string(prefix + ".ambient")), 1, glm::value_ptr(m_ambient));
        glUniform3fv(shader.getLoc(string(prefix + ".specular")), 1, glm::value_ptr(m_specular));

        glUniform1f(shader.getLoc(string(prefix + ".constant")), m_constant);
        glUniform1f(shader.getLoc(string(prefix + ".linear")), m_linear);
        glUniform1f(shader.getLoc(string(prefix + ".quadratic")), m_quadratic);

        glUniform1f(shader.getLoc(string(prefix + ".cutOffIn")), glm::cos(glm::radians(m_cutOffIn)));
        glUniform1f(shader.getLoc(string(prefix + ".curOffOut")), glm::cos(glm::radians(m_cutOffOut)));
    };

    void setPosition(glm::vec3 value) { m_position = value; }
    const glm::vec3& getPosition() { return m_position; }

    void setDirection(glm::vec3 value) { m_direction = value; }
    const glm::vec3& getDirection() { return m_direction; }

private:
    glm::vec3 m_direction;
    glm::vec3 m_position;

    float m_constant;
    float m_linear;
    float m_quadratic;

    float m_cutOffIn;
    float m_cutOffOut;
};