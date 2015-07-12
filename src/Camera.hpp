#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glmInclude.hpp"

class Camera
{
public:
	Camera();
	matrix4 getViewMatrix();
	matrix4 getProjectionMatrix(int width, int height);

    void moveCameraPos(const vector3& dist);
    void rotateCamera(const vector3& direction, float angle);
    void setCameraPos(vector3& pos);
	void setCameraTransform(vector3& position, quaternion& orientation);

    vector3 getForward();
    vector3 getRight();
    vector3 getUp();
    vector3 getPos();

private:
    vector3 forward;
    vector3 up;
    vector3 pos;

	float frameOfView = 45.0f;
	float nearClipping = 0.1f;
	float farClipping = 1000.0f;
	float isPerspective = true;

};

#endif // CAMERA_HPP
