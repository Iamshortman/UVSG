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
    matrix4 PositionMatrix = glm::translate(matrix4(1.0F), -transform.position);
    matrix4 RotationMatrix = glm::toMat4(-transform.rotation);

    return PositionMatrix * RotationMatrix;
}

