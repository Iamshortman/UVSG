#ifndef SCENE_GAME_HPP
#define SCENE_GAME_HPP

#include "Scene/Scene.hpp"
#include "World/World.hpp"
#include "Rendering/RenderingManager.hpp"

#include "Components/PlayerControl.hpp"
#include "Components/TimeToLive.hpp"

#include "World/EntityManager.hpp"
#include "Rendering/ObjLoader.hpp"

class Scene_Game : public Scene
{
public:
	Entity* camEntity = nullptr;

	Scene_Game()
	{
		this->baseWorld = new World();
		//this->baseWorld->setGravity(vector3D(0, -10, 0));

		Model* bigCubeModel = new Model();
		bigCubeModel->localOffset = Transform();
		bigCubeModel->shader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
		bigCubeModel->mesh = loadMaterialMeshFromFile("res/", "BigCube.obj");

		Entity* bigCube = EntityManager::instance()->createNewEntity();
		bigCube->addToWorld(baseWorld);
		bigCube->m_transform.m_position = vector3D(0, 0, 100);
		bigCube->addRigidBody(new RigidBody(new btBoxShape(btVector3(5.0, 5.0, 5.0)), 10000.0));
		bigCube->tempModels.push_back(bigCubeModel);

		bigCube->m_RigidBody->rigidBody->setDamping(0.5, 0.5);

		Entity* plane = EntityManager::instance()->createNewEntity();
		plane->addToWorld(baseWorld);
		plane->m_transform.m_position = vector3D(0, -1, 0);
		plane->addRigidBody(new RigidBody(new btStaticPlaneShape(btVector3(0, 1, 0), 0), 0.0));

		Model* floorModel = new Model();
		floorModel->localOffset = Transform();
		floorModel->localOffset.setScale(vector3D(20.0));
		floorModel->shader = bigCubeModel->shader;
		floorModel->mesh = loadMaterialMeshFromFile("res/Models/", "Floor.obj");

		plane->tempModels.push_back(floorModel);
	};

	virtual ~Scene_Game()
	{
		delete baseWorld;
	};

	virtual void update(double deltaTime)
	{
		baseWorld->updateWorld(deltaTime);
		Entity* entity = EntityManager::instance()->getEntity(3);
		if (entity != nullptr)
		{
			vector3D pos = entity->m_transform.getOrientation() * vector3D(0, 2, -6);
			UVSG::getInstance()->renderingManager->camera.setCameraTransform(entity->m_transform.getPos() + pos, entity->m_transform.getOrientation());
		}
	};

	virtual void render(RenderingManager* manager)
	{
		manager->window->set3dRendering();

		manager->update(0.0, baseWorld);
	};

	World* baseWorld;
};

#endif //SCENE_GAME_HPP