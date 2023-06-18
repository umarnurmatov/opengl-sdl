#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include "Keyboard.hpp"
#include "Log.hpp"


namespace Engine
{

class Camera
{
public:
    Camera();

    void processKeyboard(Keyboard& keys, float dt);

    void processMouseMove(float xrel, float yrel);

    void processMouseWheel(float y);

    glm::mat4 getView() const;

    const glm::vec3& getPos() const { return pos; };
    const glm::vec3& getFront() const { return front; }
    GLfloat getFOV() const { return fov; }

private:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    GLfloat speed;

    GLfloat fov;

    GLfloat yaw;
    GLfloat pitch;
};

};