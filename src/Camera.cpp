#include "Camera.hpp"
#include <iostream>

std::ostream &operator<< (std::ostream &out, const vector3 &vec)
{
    out << "{"
        << vec.x << ", " << vec.y << ", "<< vec.z
        << "}";

    return out;
}

Camera::Camera()
{
    pos = btVector3(0.0F, 0.0F, 0.0F);
    forward = btVector3(0.0F, 0.0F, -1.0F);
    up = btVector3(0.0F, 1.0F, 0.0F);
}

void Camera::moveCameraPos(btVector3 dist)
{
    pos += dist;
}

void Camera::rotateCamera(btVector3 direction, float angle)
{
    forward = forward.rotate(direction, angle);
    up = up.rotate(direction, angle);
    forward.normalized();
    up.normalized();
}

btVector3 Camera::getForward()
{
    return forward;
}

btVector3 Camera::getUp()
{
    return up;
}

btVector3 Camera::getRight()
{
    return forward.cross(up);
}

matrix4 Camera::getViewMatrix()
{
    return glm::lookAt
    (
        vector3(pos.getX(), pos.getY(), pos.getZ()),
        vector3(pos.getX(), pos.getY(), pos.getZ()) + vector3(forward.getX(), forward.getY(), forward.getZ()),
        vector3(up.getX(), up.getY(), up.getZ())
    );
}

