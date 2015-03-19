#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glmInclude.hpp"
#include "Transform.hpp"
#include "btBulletDynamicsCommon.h"

class Camera
{
public:
	Camera();
	matrix4 getViewMatrix();
    void moveCameraPos(btVector3 dist);
    void rotateCamera(btVector3 direction, float angle);
    void setCameraPos(vector3 pos);
    btVector3 getForward();
    btVector3 getRight();
    btVector3 getUp();

    btTransform transform;

    vector3 pos;
    quat    oriantation;

private:

};

#endif // CAMERA_HPP
