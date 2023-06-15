#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include "../MySDL/Keyboard.hpp"

namespace MyGL
{
class Camera
{
public:
    Camera();

    void processKeyboard(MySDL::Keyboard& keys, float dt);

    void processMouseMove(float xrel, float yrel);

    void processMouseWheel(float y);

    glm::mat4 getView();

    const glm::vec3& getPos() const { return m_pos; };
    const glm::vec3& getFront() const { return m_front; }
    GLfloat getFOV() const { return m_fov; }

private:
    glm::vec3 m_pos;
    glm::vec3 m_front;
    glm::vec3 m_up;
    GLfloat m_speed;

    GLfloat m_fov;

    GLfloat m_yaw;
    GLfloat m_pitch;
};
};