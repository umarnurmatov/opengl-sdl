#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include "Keyboard.hpp"

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

    glm::vec3 getPos();

    GLfloat getFOV();

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