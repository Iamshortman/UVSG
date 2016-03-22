#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "World/World.hpp"
#include "Camera.hpp"
#include "World/Entity.hpp"

class Renderer
{
	public:
		virtual void renderAmbient(World* world, Entity* entity, Camera* camera) = 0;
		virtual void renderLighting(World* world, Entity* entity, Camera* camera) = 0;
};

#endif //RENDERER_HPP