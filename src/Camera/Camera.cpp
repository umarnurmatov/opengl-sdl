#include "Camera.hpp"

namespace Engine
{

Camera::Camera()
    : pos{0.0f, 0.0f, 3.0f},
        front{0.0f, 0.0f, -1.0f},
        up{0.0f, 1.0f, 0.0f},
        pitch{0.0f},
        yaw{0.0f},
        fov{45.f}
{
}

void Camera::processKeyboard(Keyboard& keys, float dt)
{
    speed = 0.01f * dt;
    if(keys.isKeyPressed(SDLK_w))
    {
        pos += speed * front;
    }
    if(keys.isKeyPressed(SDLK_s))
    {
        pos -= speed * front;
    }
    if(keys.isKeyPressed(SDLK_a))
    {
        pos -= glm::normalize(glm::cross(front, up)) * speed;
    }
    if(keys.isKeyPressed(SDLK_d))
    {
        pos += glm::normalize(glm::cross(front, up)) * speed;
    }
}

void Camera::processMouseMove(float xrel, float yrel)
{
    GLfloat sense = 0.05f;
    xrel *= sense;
    yrel *= sense;

    yaw += xrel;
    pitch -= yrel;

    pitch = glm::clamp(pitch, -90.0f, 90.0f);

    glm::vec3 newFront;
    newFront.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
    newFront.y = glm::sin(glm::radians(pitch));
    newFront.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(newFront);
}

void Camera::processMouseWheel(float y)
{
    GLfloat sense = 0.5f;
    fov += sense * y;
}

glm::mat4 Camera::getView() const
{
    return glm::lookAt(pos, pos + front, up);
}

};