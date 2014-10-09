#ifndef CAMERA_HPP
#define CAMERA_HPP

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	glm::mat4 getOffset();

private:
    //Projection information
    glm::mat4 Projection;

    //Location and rotation
    glm::mat4 View;

};

#endif // CAMERA_HPP
