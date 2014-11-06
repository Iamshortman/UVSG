#include "Camera.hpp"

Camera::Camera()
{
    transform = Transform();
}

void Camera::moveCameraPos(vector3 dist)
{
    transform.position += dist;
}

void Camera::rotateCamera(vector3 direction, float angle)
{
    transform.rotation *= glm::normalize(glm::angleAxis(angle, direction));
}

matrix4 Camera::getViewMatrix()
{
    return glm::lookAt(transform.position, transform.position + transform.forward, transform.up);
}

