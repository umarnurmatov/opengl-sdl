#include "Camera.hpp"

namespace MyGL
{
Camera::Camera()
    : m_pos{0.0f, 0.0f, 3.0f},
        m_front{0.0f, 0.0f, -1.0f},
        m_up{0.0f, 1.0f, 0.0f},
        m_pitch{0.0f},
        m_yaw{0.0f},
        m_fov{45.f}
{
}


void Camera::processKeyboard(MySDL::Keyboard& keys, float dt)
{
    m_speed = 0.01f * dt;
    if(keys.isKeyPressed(SDLK_w))
    {
        m_pos += m_speed * m_front;
    }
    if(keys.isKeyPressed(SDLK_s))
    {
        m_pos -= m_speed * m_front;
    }
    if(keys.isKeyPressed(SDLK_a))
    {
        m_pos -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
    }
    if(keys.isKeyPressed(SDLK_d))
    {
        m_pos += glm::normalize(glm::cross(m_front, m_up)) * m_speed;
    }
}

void Camera::processMouseMove(float xrel, float yrel)
{
    GLfloat sense = 0.05f;
    xrel *= sense;
    yrel *= sense;

    m_yaw += xrel;
    m_pitch -= yrel;

    m_pitch = glm::clamp(m_pitch, -90.0f, 90.0f);

    glm::vec3 front;
    front.x = glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
    front.y = glm::sin(glm::radians(m_pitch));
    front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
}

void Camera::processMouseWheel(float y)
{
    GLfloat sense = 0.5f;
    m_fov += sense * y;
}

glm::mat4 Camera::getView()
{
    return glm::lookAt(m_pos, m_pos + m_front, m_up);
}




};