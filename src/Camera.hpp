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
    btTransform transform;

private:

};

#endif // CAMERA_HPP
