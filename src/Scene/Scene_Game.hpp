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
		//camEntity->addComponent("playerController", new PlayerControl(5.0, 0.5, SDL_GameControllerOpen(0)));
		//camEntity->setPosition(vector3D(0, 10, -10));
		//camEntity->addComponent("TempScript", new ScriptComponent());

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

		/*Entity* ship = EntityManager::instance()->createNewEntity();
		ship->addToWorld(baseWorld);
		ship->setPosition(vector3D(0, 0, 10));

		Model* shipModel = new Model();
		shipModel->localOffset = Transform();
		shipModel->shader = bigCubeModel->shader;
		shipModel->mesh = loadMaterialMeshFromFile("res/Models/", "Ship_Lightning.obj");
		ship->tempModels.push_back(shipModel);

		ship->addRigidBody(new RigidBody(new btBoxShape(btVector3(5.0, 5.0, 5.0)), 100.0));
		ship->addComponent("FlightControl", new ShipFlightControl(SDL_GameControllerOpen(0)));*/

		Entity* landingPad = EntityManager::instance()->createNewEntity();
		landingPad->addToWorld(baseWorld);
		landingPad->setPosition(vector3D(0, -10, 0));

		Model* landingPadModel = new Model();
		landingPadModel->localOffset = Transform();
		landingPadModel->shader = bigCubeModel->shader;
		landingPadModel->mesh = loadMaterialMeshFromFile("res/Models/", "LandingPad.obj");
		landingPad->tempModels.push_back(landingPadModel);
		
		btTriangleMesh* triMesh = new btTriangleMesh();
		loadTriMesh("res/Models/", "LandingPad.obj", triMesh);
		landingPad->addRigidBody(new RigidBody(new btBvhTriangleMeshShape(triMesh, true), 0.0));

		/*Entity* ground = EntityManager::instance()->createNewEntity();;
		ground->addRigidBody(new RigidBody(new btStaticPlaneShape(btVector3(0, 1, 0), 0), 0.0));
		ground->setPosition(vector3D(0, 2, 0));
		ground->addToWorld(baseWorld);*/
	};

	virtual ~Scene_Game()
	{
		delete baseWorld;
	};

	virtual void update(double deltaTime)
	{
		baseWorld->updateWorld(deltaTime);
		Entity* entity = EntityManager::instance()->getEntity(5);
		if (entity != nullptr)
		{
			vector3D pos = entity->getOrientation() * vector3D(0, 2, -6);
			UVSG::getInstance()->renderingManager->camera.setCameraTransform(entity->getPosition() + pos, entity->getOrientation());
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