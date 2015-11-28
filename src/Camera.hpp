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

    void moveCameraPos(const f64vec3& dist);
    void rotateCamera(const f64vec3& direction, double angle);
    void setCameraPos(f64vec3& pos);
	void setCameraTransform(f64vec3& position, f64quat& orientation);

    f64vec3 getForward();
    f64vec3 getRight();
    f64vec3 getUp();
    f64vec3 getPos();
	quaternion getOrientation();

	void setProjection(float frameOfView, float nearClipping, float farClipping, int screenWidth, int screenheight);

private:
    f64vec3 m_Forward;
	f64vec3 m_Up;
    f64vec3 m_Pos;

	//Perspective varables
	float isPerspective = true;
	float frameOfView = 45.0f;
	float nearClipping = 0.1f;
	float farClipping = 1000.0f;
	int width;
	int height;

};

#endif // CAMERA_HPP
