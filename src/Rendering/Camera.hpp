#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glmInclude.hpp"
#include "Util.hpp"

class Camera
{
public:
	Camera();
	matrix4 getViewMatrix();
	matrix4 getOriginViewMatrix();
	matrix4 getProjectionMatrix();

    void moveCameraPos(const vector3D& dist);
    void rotateCamera(const vector3D& direction, double angle);
    void setCameraPos(vector3D& pos);
	void setCameraTransform(vector3D position, quaternionD orientation);
	//void setCameraTransform(Transform& transform);

    vector3D getForward();
    vector3D getRight();
    vector3D getUp();
    vector3D getPos();
	quaternionF getOrientation();

	void setProjection(float frameOfView, float nearClipping, float farClipping, int screenWidth, int screenheight);

private:
    vector3D m_Forward;
	vector3D m_Up;
    vector3D m_Pos;

	//Perspective varables
	float isPerspective = true;
	float frameOfView = 45.0f;
	float nearClipping = 0.1f;
	float farClipping = 1000.0f;
	int width;
	int height;

};

#endif // CAMERA_HPP
