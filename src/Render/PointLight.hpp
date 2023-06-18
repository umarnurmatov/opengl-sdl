#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "BaseLight.hpp"


namespace Engine
{

class PointLight : public BaseLight
{
public:
    PointLight()
    :   constant{1.0f},
        linear{0.35f},
        quadratic{0.44f},
        position{1.0f}
    {}

    void render(Shader &shader, std::string prefix) override
    {
        using std::string;
        glUniform3fv(shader.getLoc(string(prefix + ".pos")), 1, glm::value_ptr(position));
        glUniform3fv(shader.getLoc(string(prefix + ".diffuse")), 1, glm::value_ptr(diffuse));
        glUniform3fv(shader.getLoc(string(prefix + ".ambient")), 1, glm::value_ptr(ambient));
        glUniform3fv(shader.getLoc(string(prefix + ".specular")), 1, glm::value_ptr(specular));

        glUniform1f(shader.getLoc(string(prefix + ".constant")), constant);
        glUniform1f(shader.getLoc(string(prefix + ".linear")), linear);
        glUniform1f(shader.getLoc(string(prefix + ".quadratic")), quadratic);
    };

    void setPosition(glm::vec3 value) { position = value; }
    const glm::vec3& getPosition() { return position; }

private:
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;
};

};