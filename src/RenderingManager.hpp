#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include <stddef.h>  // defines NULL
#include <entityxInclude.hpp>

#include "Components.hpp"
#include "VoxelComponent.hpp"

#include "Camera.hpp"
#include "Window.hpp"
#include "glmInclude.hpp"
#include "ShaderProgram.hpp"

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();
	void update(EntityX &ex, double deltaTime);

	Camera camera;
	Window* window = NULL;
	ShaderProgram basicShader;
	GLuint uniform_MVP_ID;
	GLuint uniform_Normal_ID;
	GLuint uniform_Offset_ID;

protected:

private:
	GLuint VBO, VAO;

};

#endif //RENDERINGMANAGER_HPP