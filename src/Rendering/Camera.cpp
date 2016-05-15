#include "Camera.hpp"
#include <iostream>

Camera::Camera()
{
    m_Pos = vector3F(0.0F, 0.0F, 0.0F);
    m_Forward = vector3F(0.0F, 0.0F, 1.0F);
    m_Up = vector3F(0.0F, 1.0F, 0.0F);
}

void Camera::setCameraPos(vector3D& pos)
{
	this->m_Pos = pos;
}

void Camera::moveCameraPos(const vector3D& dist)
{
    m_Pos += dist;
}

void Camera::rotateCamera(const vector3D& direction, double angle)
{
	m_Forward = glm::normalize( glm::rotate(m_Forward, angle, direction) );
	m_Up = glm::normalize( glm::rotate(m_Up, angle, direction) );

}

void Camera::setCameraTransform(vector3D position, quaternionD orientation)
{
	m_Pos = position;
	m_Forward = glm::normalize( orientation * vector3D(0.0F, 0.0F, 1.0F) );
	m_Up = glm::normalize( orientation * vector3D(0.0F, 1.0F, 0.0F) );
}

vector3D Camera::getForward()
{
    return m_Forward;
}

vector3D Camera::getUp()
{
    return m_Up;
}

vector3D Camera::getRight()
{
	return glm::cross(m_Forward, m_Up);
}

vector3D Camera::getPos()
{
    return m_Pos;
}

quaternionF Camera::getOrientation()
{
	return fromAxes((vector3F)m_Forward,(vector3F) m_Up);
}

matrix4 Camera::getViewMatrix()
{
    return glm::lookAt((vector3F) m_Pos,(vector3F) (m_Pos + m_Forward),(vector3F) m_Up);
}

matrix4 Camera::getOriginViewMatrix()
{
	vector3F forward = (vector3F)m_Forward;
	vector3F up = (vector3F)m_Up;

	return glm::lookAt(vector3F(0.0f), forward, up);
}

matrix4 Camera::getProjectionMatrix()
{
	float aspectRatio = ((float)width) / ((float)height);
	return glm::infinitePerspective(frameOfView, aspectRatio, nearClipping);
}

matrix4 Camera::getModelMatrix()
{
	matrix4 positionMatrix = matrix4();
	matrix4 rotationMatrix = matrix4();

	positionMatrix = glm::translate(matrix4(1.0F), (vector3F)m_Pos);
	rotationMatrix = glm::toMat4(this->getOrientation());

	return positionMatrix * rotationMatrix;
}

void Camera::setProjection(float frameOfView, float nearClipping, float farClipping, int screenWidth, int screenHeight)
{
	this->frameOfView = frameOfView;
	this->nearClipping = nearClipping;
	this->farClipping = farClipping;
	this->width = screenWidth;
	this->height = screenHeight;
}
