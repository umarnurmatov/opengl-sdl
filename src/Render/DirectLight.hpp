#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

#include "BaseLight.hpp"

namespace Engine
{

class DirectLight : public BaseLight
{
public:
    DirectLight()
    :   direction{0.0f}
    {}

    void render(Shader &shader, std::string prefix = "directLight") override
    {
        using std::string;
        glUniform3fv(shader.getLoc(string(prefix + ".dir")), 1, glm::value_ptr(direction));
        glUniform3fv(shader.getLoc(string(prefix + ".diffuse")), 1, glm::value_ptr(diffuse));
        glUniform3fv(shader.getLoc(string(prefix + ".ambient")), 1, glm::value_ptr(ambient));
        glUniform3fv(shader.getLoc(string(prefix + ".specular")), 1, glm::value_ptr(specular));
    };

    void setDirection(glm::vec3 value) { direction = value; }
    const glm::vec3& getDirection() { return direction; }

private:
    glm::vec3 direction;
};

};