#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include <stddef.h>  // defines NULL
#include "entityxInclude.hpp"

#include "Components.hpp"

#include "Camera.hpp"
#include "Window.hpp"
#include "glmInclude.hpp"
#include "ShaderProgram.hpp"
#include "TexturePool.hpp"
#include "Chunk.hpp"
#include "TestChunk.hpp"

struct Star
{
	Transform transform;
	Mesh starMesh;
	Mesh billboardMesh;
	vector3 color;
};

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
	ShaderProgram texturedShader;

	ShaderProgram texturedLightShader;

	ShaderProgram StarShader;
	ShaderProgram BillboardShader;

	//TestChunk chunk;
	Star m_sun;

protected:

private:
	GLuint VBO, VAO;

};

#endif //RENDERINGMANAGER_HPP