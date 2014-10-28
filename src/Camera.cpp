#include "Camera.hpp"

Camera::Camera()
{

	// Camera matrix
	/*View        = matrix4(1.0F); lookAt(
								vector3(0,3,3), // Camera is at (4,3,3), in World Space
								vector3(0,0,0), // and looks at the origin
								vector3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );*/
    //this->moveCameraPos(vector3(0,0,-3));
}

void Camera::moveCameraPos(vector3 dist)
{
    //View = glm::translate(View, dist);
}

void Camera::rotateCamera(vector3 direction, float angle)
{
    //View = glm::rotate(View, angle, direction);
}

matrix4 Camera::getViewMatrix()
{
    return matrix4();
}

