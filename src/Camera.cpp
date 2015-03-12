#include "Camera.hpp"
#include <iostream>

Camera::Camera()
{
    transform = Transform();
}

void Camera::moveCameraPos(vector3 dist)
{
    transform.translate(dist);
}

void Camera::rotateCamera(vector3 direction, float angle)
{
    quat rotation = glm::angleAxis(angle, direction);
    std::cout << "{"
        << rotation.x << ", " << rotation.y << ","<< rotation.z << "," << rotation.w
        << "}" << std::endl;

//    transform.rotation *= glm::normalize(quat);
}

matrix4 Camera::getViewMatrix()
{
    matrix4 transformMatrix = glm::translate(matrix4(1.0f), this->transform.getPosition() * -1.0f);
    return glm::mat4();
}

