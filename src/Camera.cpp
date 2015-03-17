#include "Camera.hpp"
#include <iostream>

Camera::Camera()
{
    transform = btTransform();
}

void Camera::moveCameraPos(btVector3 dist)
{
    transform.getOrigin() += dist;
}

void Camera::rotateCamera(btVector3 direction, float angle)
{
    btQuaternion angleToRotate = btQuaternion(direction, angle);
    transform.setRotation(transform.getRotation() * angleToRotate); //Should rotate the object by the axis angle.
}

matrix4 Camera::getViewMatrix()
{
    btVector3 origin = transform.getOrigin();
    matrix4 transformMatrix = glm::translate( matrix4(1.0f), vector3(origin.getX(), origin.getY(), origin.getZ()) * -1.0f);
    btQuaternion rotate = transform.getRotation();
    rotate.inverse();
    matrix4 orientationMatrix = glm::toMat4( quat(rotate.getX(), rotate.getY(), rotate.getZ(), rotate.getW()) );

    return transformMatrix ;//* orientationMatrix;
}

