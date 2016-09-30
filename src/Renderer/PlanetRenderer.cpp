#include "Renderer/PlanetRenderer.hpp"
#include "Rendering/ObjLoader.hpp"

PlanetRenderer::PlanetRenderer()
{
	mesh = loadMaterialMeshFromFile("res/Models/", "Planet.obj");
	ambientShader = new ShaderProgram("res/Shaders/Material.Deferred.vs", "res/Shaders/Material.Deferred.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
}

PlanetRenderer::~PlanetRenderer()
{
	delete mesh;
	delete ambientShader;
	delete lightingShader;
}

void PlanetRenderer::renderAmbient(World* world, Entity* entity, Camera* camera)
{
	const double farViewScaleValue = 100000.0;

	Transform transform = entity->getRenderTransform();

	matrix4 projectionMatrix = camera->getProjectionMatrix();
	matrix4 viewMatrix = camera->getOriginViewMatrix();

	matrix4 modelMatrix = transform.getModleMatrix(camera->getPosition(), farViewScaleValue);
	matrix3 normalMatrix = transform.getNormalMatrix();
	matrix4 MVP = projectionMatrix * viewMatrix * modelMatrix;

	ambientShader->setActiveProgram();

	ambientShader->setUniform("MVP", MVP);
	ambientShader->setUniform("normalMatrix", normalMatrix);
	ambientShader->setUniform("modelMatrix", modelMatrix);
	ambientShader->setUniform("ambientLight", world->ambientLight);

	mesh->draw(ambientShader);

	ambientShader->deactivateProgram();
}

void PlanetRenderer::renderTransparency(World* world, Entity* entity, Camera* camera)
{

}

void PlanetRenderer::renderLighting(World* world, Entity* entity, Camera* camera)
{

}

RenderView PlanetRenderer::getRenderView()
{
	return FarView;
}