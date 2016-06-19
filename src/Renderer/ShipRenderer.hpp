#ifndef ENTITYRENDERER_HPP
#define ENTITYRENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Renderable.hpp"

class DirectionalLight;

class ShipRenderer : public Renderer
{
public:
	ShipRenderer();
	~ShipRenderer();

	virtual void renderAmbient(World* world, Entity* entity, Camera* camera);
	virtual void renderTransparency(World* world, Entity* entity, Camera* camera);
	virtual void renderLighting(World* world, Entity* entity, Camera* camera);
	virtual RenderView getRenderView();

private:

	ShaderProgram* materialShader = nullptr;

	DirectionalLight* directionalLight = nullptr;
	ShaderProgram* DirectionalShader = nullptr;
};

#endif //ENTITYRENDERER_HPP