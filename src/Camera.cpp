#include "Camera.hpp"
#include <iostream>

Camera::Camera()
{
    m_Pos = vector3(0.0F, 0.0F, 0.0F);
    m_Forward = vector3(0.0F, 0.0F, 1.0F);
    m_Up = vector3(0.0F, 1.0F, 0.0F);
}

void Camera::setCameraPos(f64vec3& pos)
{
	this->m_Pos = pos;
}

void Camera::moveCameraPos(const f64vec3& dist)
{
    m_Pos += dist;
}

void Camera::rotateCamera(const f64vec3& direction, double angle)
{
	m_Forward = glm::normalize( glm::rotate(m_Forward, angle, direction) );
	m_Up = glm::normalize( glm::rotate(m_Up, angle, direction) );

}

void Camera::setCameraTransform(f64vec3& position, f64quat& orientation)
{
	m_Pos = position;
	m_Forward = glm::normalize( orientation * f64vec3(0.0F, 0.0F, 1.0F) );
	m_Up = glm::normalize( orientation * f64vec3(0.0F, 1.0F, 0.0F) );
}

f64vec3 Camera::getForward()
{
    return m_Forward;
}

f64vec3 Camera::getUp()
{
    return m_Up;
}

f64vec3 Camera::getRight()
{
	return glm::cross(m_Forward, m_Up);
}

f64vec3 Camera::getPos()
{
    return m_Pos;
}

quaternion Camera::getOrientation()
{
	return fromAxes(m_Forward, m_Up);
}

matrix4 Camera::getViewMatrix()
{
    return glm::lookAt(m_Pos, m_Pos + m_Forward, m_Up);
}

matrix4 Camera::getOriginViewMatrix()
{
	vector3 forward = (vector3)m_Forward;
	vector3 up = (vector3)m_Up;

	return glm::lookAt(vector3(0.0f), forward, up);
}

matrix4 Camera::getProjectionMatrix()
{
	float aspectRatio = ((float)width) / ((float)height);
	return glm::perspective(frameOfView, aspectRatio, nearClipping, farClipping);
}

void Camera::setProjection(float frameOfView, float nearClipping, float farClipping, int screenWidth, int screenHeight)
{
	this->frameOfView = frameOfView;
	this->nearClipping = nearClipping;
	this->farClipping = farClipping;
	this->width = screenWidth;
	this->height = screenHeight;
}