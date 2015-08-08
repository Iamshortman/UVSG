#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include <stddef.h>  // defines NULL
#include <entityx\entityx.h>

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
	void update(entityx::EntityX &ex, float deltaTime);
	matrix4 createModelMatrix(entityx::Entity entity);

	Camera camera;
	Window* window = NULL;
	ShaderProgram basicShader;
	GLuint uniform_MVP_ID;

protected:

private:
	GLuint VBO, VAO;

};

#endif //RENDERINGMANAGER_HPP