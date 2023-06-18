#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

#include "BaseLight.hpp"


namespace Engine
{

class SpotLight : public BaseLight
{
public:
    SpotLight()
    :   constant{1.0f},
        linear{0.022f},
        quadratic{0.0019f},

        position{1.0f},
        direction{1.0f}
    {
        setCutOff(12.5f, 17.5f);
    }

    void render(Shader &shader, std::string prefix) override
    {
        using std::string;
        glUniform3fv(shader.getLoc(string(prefix + ".pos")), 1, glm::value_ptr(position));
        glUniform3fv(shader.getLoc(string(prefix + ".dir")), 1, glm::value_ptr(direction));

        glUniform3fv(shader.getLoc(string(prefix + ".diffuse")), 1, glm::value_ptr(diffuse));
        glUniform3fv(shader.getLoc(string(prefix + ".ambient")), 1, glm::value_ptr(ambient));
        glUniform3fv(shader.getLoc(string(prefix + ".specular")), 1, glm::value_ptr(specular));

        glUniform1f(shader.getLoc(string(prefix + ".constant")), constant);
        glUniform1f(shader.getLoc(string(prefix + ".linear")), linear);
        glUniform1f(shader.getLoc(string(prefix + ".quadratic")), quadratic);

        glUniform1f(shader.getLoc(string(prefix + ".cutOffIn")), cutOffIn);
        glUniform1f(shader.getLoc(string(prefix + ".cutOffOut")), cutOffOut);
    };

    void setPosition(glm::vec3 value) { position = value; }
    const glm::vec3& getPosition() const { return position; }

    void setDirection(glm::vec3 value) { direction = value; }
    const glm::vec3& getDirection() const { return direction; }

    void setCutOff(float cutOffIn_deg, float cutOffOut_deg) { cutOffIn = glm::cos(glm::radians(cutOffIn_deg)); cutOffOut = glm::cos(glm::radians(cutOffOut_deg)); }

private:
    glm::vec3 direction;
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    float cutOffIn;
    float cutOffOut;
};

};