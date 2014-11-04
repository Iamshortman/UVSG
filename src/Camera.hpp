#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glmInclude.hpp"
#include "Transform.hpp"

class Camera
{
public:
	Camera();
	matrix4 getViewMatrix();
    void moveCameraPos(vector3 dist);
    void rotateCamera(vector3 direction, float angle);
    void setCameraPos(vector3 pos);

private:
    Transform transform;

};

#endif // CAMERA_HPP
