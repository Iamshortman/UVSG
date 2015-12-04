#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Components.hpp"

#include "Camera.hpp"
#include "Window.hpp"
#include "glmInclude.hpp"
#include "ShaderProgram.hpp"
#include "TexturePool.hpp"
#include "EntityxInclude.hpp"

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();
	void update(EntityX &entitySystem, double deltaTime);

	Camera camera;
	Window* window = NULL;
	TexturePool texturePool;

	const double farViewScaleValue = 10000.0;


protected:

private:

};

#endif //RENDERINGMANAGER_HPP