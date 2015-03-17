#include "Camera.hpp"
#include <iostream>

std::ostream &operator<< (std::ostream &out, const quat &vec)
{
    out << "{"
        << vec.x << ", " << vec.y << ", "<< vec.z << ", " << vec.w
        << "}";

    return out;
}

Camera::Camera()
{
    transform = btTransform();
    transform.setRotation(btQuaternion(0.0F, 0.0F, 0.0F, 1.0F));
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

    btQuaternion rotation = transform.getRotation();
    quat tempRotation = quat(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW());
    std::cout << tempRotation << std::endl;
    matrix4 orientationMatrix = glm::toMat4(tempRotation);

    return transformMatrix * orientationMatrix;
}

