#ifndef RAYTRACINGDEBUG_HPP
#define RAYTRACINGDEBUG_HPP

#include "Renderer/Renderer.hpp"
#include "Renderable.hpp"
#include "Rendering/ObjLoader.hpp"

class DirectionalLight;

class RayTracingDebug : public Renderer
{
public:
	RayTracingDebug()
	{
		materialShader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
		mesh = loadMaterialMeshFromFile("res/", "point.obj");
	};
	~RayTracingDebug()
	{

	};

	virtual void renderAmbient(World* world, Entity* entity, Camera* camera)
	{
		Transform entityTransform = entity->getTransform();
		Transform worldOffset = world->getWorldOffsetMatrix();

		Transform localOffset;

		quaternionD rot = entityTransform.getOrientation();
		vector3D pos = entityTransform.getPosition();
		vector3D down = -entityTransform.getUp();

		double rayDist = 20.0;
		double stepLen = 0.5;
		int count = 10;

		materialShader->setActiveProgram();
		materialShader->setUniform("ambientLight", vector3F(1.0f));

		for (int x = -count; x < count; x++)
		{
			for (int y = -count; y < count; y++)
			{
				vector3D rayStart = (rot * vector3D(x * stepLen, 0, y * stepLen) + pos);
				vector3D rayEnd = rayStart + (down * rayDist);

				SingleRayTestResult result = world->m_physicsWorld->singleRayTest(rayStart, rayEnd);
				if (result.hasHit)
				{
					localOffset = Transform(result.hitPosition - pos);
				}
				else
				{
					localOffset = Transform(rayEnd - pos);
				}

				matrix4 projectionMatrix = camera->getProjectionMatrix();
				matrix4 viewMatrix = camera->getOriginViewMatrix();

				matrix4 modelMatrix = entityTransform.getModleMatrix(camera->getPosition());
				matrix3 normalMatrix = entityTransform.getNormalMatrix();

				modelMatrix = modelMatrix * localOffset.getModleMatrix();
				normalMatrix = normalMatrix * localOffset.getNormalMatrix();

				matrix4 MVP = projectionMatrix * viewMatrix * modelMatrix;

				materialShader->setUniform("MVP", MVP);
				materialShader->setUniform("normalMatrix", normalMatrix);


				mesh->draw(materialShader);

			}
		}

		materialShader->deactivateProgram();

	};
	virtual void renderTransparency(World* world, Entity* entity, Camera* camera){};
	virtual void renderLighting(World* world, Entity* entity, Camera* camera){};
	virtual RenderView getRenderView()
	{
		return NearView;
	};

private:

	ShaderProgram* materialShader = nullptr;
	Mesh* mesh = nullptr;
};

#endif //RAYTRACINGDEBUG_HPP