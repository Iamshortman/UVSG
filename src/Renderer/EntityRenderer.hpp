#ifndef ENTITYRENDERER_HPP
#define ENTITYRENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Renderable.hpp"

class DirectionalLight;

class EntityRenderer : public Renderer
{
public:
	EntityRenderer();
	~EntityRenderer();

	virtual void renderAmbient(World* world, Entity* entity, Camera* camera);
	virtual void renderTransparency(World* world, Entity* entity, Camera* camera);
	virtual void renderLighting(World* world, Entity* entity, Camera* camera);
	virtual RenderView getRenderView();

private:

	DirectionalLight* directionalLight;
	ShaderProgram* DirectionalShader;
};

#endif //ENTITYRENDERER_HPP