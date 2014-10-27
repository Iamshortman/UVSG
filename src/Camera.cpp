#include "Camera.hpp"

Camera::Camera()
{

	// Camera matrix
	View        = glm::lookAt(
								glm::vec3(0,3,3), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
}

void Camera::moveCameraPos(glm::vec3 dist)
{
    View = glm::translate(View, dist);
}

glm::mat4 Camera::getViewMatrix()
{
    return View;
}

