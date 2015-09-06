#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include <stddef.h>  // defines NULL
#include "entityxInclude.hpp"

#include "Components.hpp"
#include "VoxelComponent.hpp"

#include "Camera.hpp"
#include "Window.hpp"
#include "glmInclude.hpp"
#include "ShaderProgram.hpp"
#include "TexturePool.hpp"

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();
	void update(EntityX &ex, double deltaTime);

	Camera camera;
	Window* window = NULL;
	TexturePool texturePool;

	ShaderProgram basicShader;
	GLuint uniform_MVP_ID;
	GLuint uniform_Normal_ID;
	GLuint uniform_Offset_ID;

	ShaderProgram texturedShader;
	GLuint uniform_MVP_ID1;
	GLuint uniform_Normal_ID1;

protected:

private:
	GLuint VBO, VAO;

};

#endif //RENDERINGMANAGER_HPP