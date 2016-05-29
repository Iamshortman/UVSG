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

		Entity* camEntity = EntityManager::instance()->createNewEntity();
		camEntity->addToWorld(baseWorld);
		camEntity->addComponent("playerController", new PlayerControl(5.0, 1.0, SDL_GameControllerOpen(0)));

 		Model* bigCubeModel = new Model();
		bigCubeModel->localOffset = Transform();
		bigCubeModel->shader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
		bigCubeModel->mesh = loadMaterialMeshFromFile("res/", "BigCube.obj");

		Entity* bigCube = EntityManager::instance()->createNewEntity();
		bigCube->addToWorld(baseWorld);
		bigCube->setPosition(vector3D(0, 0, 100));
		bigCube->addRigidBody(new RigidBody(new btBoxShape(btVector3(5.0, 5.0, 5.0)), 100.0));
		bigCube->tempModels.push_back(bigCubeModel);

		Entity* planet = EntityManager::instance()->createNewEntity();
		planet->addToWorld(baseWorld);
		planet->setPosition(vector3D(50000.0, -1000000.0, 30000000.0));
		planet->setScale(vector3D(30000000.0));
		planet->addComponent("planet", new Component());
		planet->setOrientation(quaternionD(0.963, -0.164, -0.202, -0.067));
	};

	virtual ~Scene_Game()
	{
		delete baseWorld;
	};

	virtual void update(double deltaTime)
	{
		baseWorld->updateWorld(deltaTime);
		Entity* entity = EntityManager::instance()->getEntity(1);
		if (entity != nullptr)
		{
			UVSG::getInstance()->renderingManager->camera.setCameraTransform(entity->getPosition(), entity->getOrientation());
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