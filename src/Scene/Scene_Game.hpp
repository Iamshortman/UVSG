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

		camEntity = EntityManager::instance()->createNewEntity();

		vector3D cameraPos = UVSG::getInstance()->renderingManager->camera.getPos();
		//quaternionD cameraRot = UVSG::getInstance()->renderingManager->camera.getOrientation();

		camEntity->m_transform.m_position = cameraPos;
		//camEntity->m_transform.m_orientation = cameraRot;
		//camEntity->addComponent("PlayerControl", new PlayerControl(5.0, 3.0, SDL_GameControllerOpen(0)));

		//camEntity->addToWorld(baseWorld);

		Model* smallCubeModel = new Model();
		smallCubeModel->localOffset = Transform();
		smallCubeModel->shader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
		smallCubeModel->mesh = loadMaterialMeshFromFile("res/", "SmallCube.obj");

		/*for (int x = -5; x < 5; x++)
		{
			for (int z = -5; z < 5; z++)
			{
				Entity* smallCube = EntityManager::instance()->createNewEntity();
				smallCube->addToWorld(baseWorld);
				smallCube->m_transform.m_position = vector3D(x, 20, z) * 0.5;
				smallCube->addRigidBody(new RigidBody(new btBoxShape(btVector3(0.05, 0.05, 0.05)), 10.0));
				smallCube->tempModels.push_back(smallCubeModel);
				smallCube->m_Velocity.linearVelocity = vector3D(0, -10, 0);

				smallCube->addComponent("TimeToLive", new TimeToLive(20.0));
			}
		}*/

	};

	virtual ~Scene_Game()
	{
		delete baseWorld;
	};

	virtual void update(double deltaTime)
	{
		baseWorld->updateWorld(deltaTime);

		//UVSG::getInstance()->renderingManager->camera.setCameraTransform(camEntity->m_transform.getPos(), camEntity->m_transform.getOrientation());
	};

	virtual void render(RenderingManager* manager)
	{
		manager->window->set3dRendering();

		manager->update(0.0, baseWorld);
	};

	World* baseWorld;
};

#endif //SCENE_GAME_HPP