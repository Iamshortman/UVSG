#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "World/World.hpp"
#include "Rendering/Camera.hpp"
#include "World/Entity.hpp"

enum RenderView
{
	NearView,
	FarView,
};

class Renderer
{
	public:
		virtual void renderAmbient(World* world, Entity* entity, Camera* camera) = 0;
		virtual void renderTransparency(World* world, Entity* entity, Camera* camera) = 0;
		virtual void renderLighting(World* world, Entity* entity, Camera* camera) = 0;
		virtual RenderView getRenderView() = 0;
};

#endif //RENDERER_HPP