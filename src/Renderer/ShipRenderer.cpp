#include "Renderer/ShipRenderer.hpp"
#include "Rendering/RenderingManager.hpp"
#include "Ship/ShipComponent.hpp"

ShipRenderer::ShipRenderer()
{
	materialShader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });

	directionalLight = new DirectionalLight(vector3F(1.0f, -1.0f, 1.0f), vector3F(1.0f, 1.0f, 1.0f), 0.6f);
	DirectionalShader = new ShaderProgram("res/Material.vs", "res/foward-directional.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
}

ShipRenderer::~ShipRenderer()
{
	delete materialShader;
	delete directionalLight;
	delete DirectionalShader;
}

void ShipRenderer::renderAmbient(World* world, Entity* entity, Camera* camera)
{
	vector3F ambientLight = world->ambientLight;

	Transform entityTransform = entity->getTransform();
	Transform worldOffset = world->getWorldOffsetMatrix();

	Transform localOffset;

	ShipComponent* ship = (ShipComponent*)entity->getComponent("shipComponent");

	materialShader->setActiveProgram();

	Mesh* outsideMesh = ship->outsideMesh;
	if (outsideMesh != nullptr)
	{
		localOffset = Transform(-ship->m_centerOfMass);
		matrix4 projectionMatrix = camera->getProjectionMatrix();
		matrix4 viewMatrix = camera->getOriginViewMatrix();

		matrix4 modelMatrix = entityTransform.getModleMatrix(camera->getPosition());
		matrix3 normalMatrix = entityTransform.getNormalMatrix();

		modelMatrix = modelMatrix * localOffset.getModleMatrix();
		normalMatrix = normalMatrix * localOffset.getNormalMatrix();

		matrix4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		materialShader->setUniform("MVP", MVP);
		materialShader->setUniform("normalMatrix", normalMatrix);
		materialShader->setUniform("ambientLight", ambientLight);

		outsideMesh->draw(materialShader);
	}

	//For each cell in the ship
	for (auto it : ship->m_shipCells)
	{
		Mesh* mesh = it.second.getMesh();
		if (mesh != nullptr)
		{
			localOffset = Transform(((vector3D)it.first) - ship->m_centerOfMass);
			matrix4 projectionMatrix = camera->getProjectionMatrix();
			matrix4 viewMatrix = camera->getOriginViewMatrix();

			matrix4 modelMatrix = entityTransform.getModleMatrix(camera->getPosition());
			matrix3 normalMatrix = entityTransform.getNormalMatrix();

			modelMatrix = modelMatrix * localOffset.getModleMatrix();
			normalMatrix = normalMatrix * localOffset.getNormalMatrix();

			matrix4 MVP = projectionMatrix * viewMatrix * modelMatrix;

			materialShader->setUniform("MVP", MVP);
			materialShader->setUniform("normalMatrix", normalMatrix);
			materialShader->setUniform("ambientLight", ambientLight);

			mesh->draw(materialShader);
		}
	}

	materialShader->deactivateProgram();

}

void ShipRenderer::renderLighting(World* world, Entity* entity, Camera* camera)
{

}

RenderView ShipRenderer::getRenderView()
{
	return NearView;
}