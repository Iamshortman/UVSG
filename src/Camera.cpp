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
    pos = vector3(0.0F, 0.0F, 0.0F);
    forward = vector3(0.0F, 0.0F, 1.0F);
    up = vector3(0.0F, 1.0F, 0.0F);
}

void Camera::setCameraPos(vector3& pos)
{
	this->pos = vector3(pos.x, pos.y, pos.z);
}

void Camera::moveCameraPos(const vector3& dist)
{
    pos += dist;
}

void Camera::rotateCamera(const vector3& direction, float angle)
{
	forward = glm::normalize( glm::rotate(forward, angle, direction) );
	up = glm::normalize( glm::rotate(up, angle, direction) );

}

void Camera::setCameraTransform(vector3& position, quaternion& orientation)
{
	pos = position;
	forward = glm::normalize( orientation * vector3(0.0F, 0.0F, 1.0F) );
	up = glm::normalize( orientation * vector3(0.0F, 1.0F, 0.0F) );
}

vector3 Camera::getForward()
{
    return forward;
}

vector3 Camera::getUp()
{
    return up;
}

vector3 Camera::getRight()
{
	return glm::cross(forward, up);
}

vector3 Camera::getPos()
{
    return pos;
}

matrix4 Camera::getViewMatrix()
{
    return glm::lookAt(pos, pos + forward, up);
}

matrix4 Camera::getProjectionMatrix(int width, int height)
{
	float aspectRatio = ((float)width) / ((float)height);
	return glm::perspective(frameOfView, aspectRatio, nearClipping, farClipping);
}
