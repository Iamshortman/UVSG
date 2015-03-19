#include "Camera.hpp"
#include <iostream>

Camera::Camera()
{
    transform = btTransform();
    transform.setRotation(btQuaternion(0.0F, 0.0F, 0.0F, 1.0F));
}

void Camera::moveCameraPos(btVector3 dist)
{
     transform.setOrigin(transform.getOrigin() + dist);
}

void Camera::rotateCamera(btVector3 direction, float angle)
{
    btQuaternion angleToRotate = btQuaternion(direction, angle);
    transform.setRotation(transform.getRotation() * angleToRotate); //Should rotate the object by the axis angle.
    transform.setRotation(transform.getRotation().normalized());
}

btVector3 Camera::getForward()
{
    btVector3 vec = btVector3(matrix[0][2], matrix[1][2], matrix[2][2]);
    return vec * 1.0F;
}

btVector3 Camera::getRight()
{
    matrix4 matrix;
    transform.getOpenGLMatrix(&matrix[0][0]);
    btVector3 vec = btVector3(matrix[0][0], matrix[0][1], matrix[0][2]);
    return btVector3();
}

btVector3 Camera::getUp()
{
    matrix4 matrix;
    transform.getOpenGLMatrix(&matrix[0][0]);
    btVector3 vec = btVector3(matrix[1][0], matrix[1][1], matrix[1][2]);
    return btVector3();
}

matrix4 Camera::getViewMatrix()
{
    btVector3 origin = transform.getOrigin();
    matrix4 transformMatrix = glm::translate( matrix4(1.0f), vector3(origin.getX(), origin.getY(), origin.getZ()) * -1.0f);

    btQuaternion rotation = transform.getRotation();
    quat tempRotation = quat(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW());
    matrix4 orientationMatrix = glm::toMat4(tempRotation);

    return orientationMatrix * transformMatrix;
}

