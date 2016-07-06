#ifndef SCENE_GAME_HPP
#define SCENE_GAME_HPP

#include "Scene/Scene.hpp"
#include "World/World.hpp"
#include "Rendering/RenderingManager.hpp"

#include "Components/ShipFlightControl.hpp"
#include "Components/PlayerControl.hpp"
#include "Components/TimeToLive.hpp"
#include "Components/ScriptComponent.hpp"

#include "World/EntityManager.hpp"
#include "Rendering/ObjLoader.hpp"

#include "Renderer/RayTracingDebug.hpp"

class Scene_Game : public Scene
{
public:
	Entity* cam_Entity = nullptr;

	Scene_Game()
	{
		this->baseWorld = new World();
		//this->baseWorld->setGravity(vector3D(0, -10, 0));

		Entity* camEntity = EntityManager::Instance->createNewEntity();
		camEntity->addToWorld(baseWorld);
		camEntity->addComponent("playerController", new PlayerControl(5.0, 0.5, SDL_GameControllerOpen(0)));
		Transform transform;
		transform.setPosition(vector3D(0, 10, -10));
		camEntity->setTransform(transform);
		cam_Entity = camEntity;

 		Model* bigCubeModel = new Model();
		bigCubeModel->localOffset = Transform();
		bigCubeModel->shader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
		bigCubeModel->mesh = loadMaterialMeshFromFile("res/", "BigCube.obj");

		transform = Transform();
		Entity* bigCube = EntityManager::Instance->createNewEntity();
		bigCube->addToWorld(baseWorld);
		transform.setPosition(vector3D(0, 10, 100));
		bigCube->setTransform(transform);
		bigCube->addRigidBody(new RigidBody(new btBoxShape(btVector3(5.0, 5.0, 5.0)), 100.0));
		bigCube->tempModels.push_back(bigCubeModel);
		bigCube->setDampening(0.5, 0.5);

		transform = Transform();
		Entity* planet = EntityManager::Instance->createNewEntity();
		planet->addToWorld(baseWorld);
		planet->addComponent("planet", new Component());

		transform.setPosition(vector3D(50000.0, -1000000.0, 30000000.0));
		transform.setScale(vector3D(30000000.0));
		transform.setOrientation(quaternionD(0.963, -0.164, -0.202, -0.067));
		planet->setTransform(transform);

		/*Entity* ship = EntityManager::Instance->createNewEntity();
		ship->addToWorld(baseWorld);
		ship->setPosition(vector3D(0, 0, 10));
		Model* shipModel = new Model();
		shipModel->localOffset = Transform();
		shipModel->shader = bigCubeModel->shader;
		shipModel->mesh = loadMaterialMeshFromFile("res/Models/", "Ship_Lightning.obj");
		ship->tempModels.push_back(shipModel);
		ship->addRigidBody(new RigidBody(new btBoxShape(btVector3(5.0, 5.0, 5.0)), 100.0));
		ship->addComponent("FlightControl", new ShipFlightControl(SDL_GameControllerOpen(0)));*/

		transform = Transform();
		Entity* landingPad = EntityManager::Instance->createNewEntity();
		landingPad->addToWorld(baseWorld);
		transform.setPosition(vector3D(0, -10, 0));
		landingPad->setTransform(transform);

		Model* landingPadModel = new Model();
		landingPadModel->localOffset = Transform();
		landingPadModel->shader = bigCubeModel->shader;
		landingPadModel->mesh = loadMaterialMeshFromFile("res/Models/", "LandingPad.obj");
		landingPad->tempModels.push_back(landingPadModel);
		
		btTriangleMesh* triMesh = new btTriangleMesh();
		loadTriMesh("res/Models/", "LandingPad.obj", triMesh);
		landingPad->addRigidBody(new RigidBody(new btBvhTriangleMeshShape(triMesh, true), 0.0));

		/*Entity* ground = EntityManager::Instance->createNewEntity();;
		ground->addRigidBody(new RigidBody(new btStaticPlaneShape(btVector3(0, 1, 0), 0), 0.0));
		ground->setPosition(vector3D(0, 2, 0));
		ground->addToWorld(baseWorld);*/


		/*Entity* debugRay = EntityManager::Instance->createNewEntity();
		debugRay->addToWorld(baseWorld);
		transform.setPosition(vector3D(0, 20, 0));
		debugRay->setTransform(transform);
		debugRay->m_renderer = new RayTracingDebug();*/
	};

	virtual ~Scene_Game()
	{
		delete baseWorld;
	};

	virtual void update(double deltaTime)
	{
		baseWorld->updateWorld(deltaTime);
		if (cam_Entity != nullptr)
		{
			Transform transform = cam_Entity->getTransform();
			UVSG::getInstance()->renderingManager->camera.setCameraTransform(transform.getPosition(), transform.getOrientation());
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