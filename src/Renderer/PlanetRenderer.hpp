#ifndef PLANETRENDERER_HPP
#define PLANETRENDERER_HPP

#include "Renderer/Renderer.hpp"
#include "Renderable.hpp"

class PlanetRenderer : public Renderer
{
public:
	PlanetRenderer();
	~PlanetRenderer();

	virtual void renderAmbient(World* world, Entity* entity, Camera* camera);
	virtual void renderLighting(World* world, Entity* entity, Camera* camera);
	virtual RenderView getRenderView();

private:
	Mesh* mesh = nullptr;
	ShaderProgram* ambientShader = nullptr;
	ShaderProgram* lightingShader = nullptr;

};

#endif //PLANETRENDERER_HPP