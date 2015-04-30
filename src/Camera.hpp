#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glmInclude.hpp"
#include "btBulletDynamicsCommon.h"

class Camera
{
public:
	Camera();
	matrix4 getViewMatrix();
    void moveCameraPos(const btVector3& dist);
    void rotateCamera(const btVector3& direction, float angle);
    void setCameraPos(vector3& pos);
    btVector3 getForward();
    btVector3 getRight();
    btVector3 getUp();
    btVector3 getPos();

    btTransform transform;

private:
    btVector3 forward;
    btVector3 up;
    btVector3 pos;

};

#endif // CAMERA_HPP
