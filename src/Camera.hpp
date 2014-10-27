#ifndef CAMERA_HPP
#define CAMERA_HPP

// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	glm::mat4 getViewMatrix();
    void moveCameraPos(glm::vec3 dist);
    void setCameraPos(glm::vec3 pos);

private:
    //Location and rotation
    glm::mat4 View;

};

#endif // CAMERA_HPP
