#ifndef SCENE_GAME_HPP
#define SCENE_GAME_HPP

#include "Scene/Scene.hpp"
#include "World/World.hpp"
#include "Rendering/RenderingManager.hpp"
#include "Renderer/ShipRenderer.hpp"

#include "Components/ShipFlightControl.hpp"
#include "Components/PlayerControl.hpp"
#include "Components/TimeToLive.hpp"
#include "Components/ScriptComponent.hpp"
#include "Ship/ShipComponent.hpp"

#include "World/EntityManager.hpp"
#include "Rendering/ObjLoader.hpp"

#include "Renderer/RayTracingDebug.hpp"

#include "Physics/MeshCollisionShape.hpp"

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
		camEntity->addComponent("playerController", new PlayerControl(5.0, 0.5));
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
		transform.setPosition(vector3D(0, -10, 100));
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

		/*ShipComponent* shipComponent = new ShipComponent(1.0);
		ShipCell* hullCell = UVSG::getInstance()->shipCellDictionary->getCell("smallship_hull");

		for (int i = -126; i < 126; i++)
		{
			shipComponent->addCell(ShipCellData(hullCell, vector3B(i,0,0)));
		}
		Entity* ship = EntityManager::Instance->createNewEntity();
		ship->addToWorld(baseWorld);
		ship->setTransform(Transform(vector3D(0, 10, 150)));
		ship->m_renderer = new ShipRenderer();
		ship->addComponent("shipComponent", shipComponent);
		shipComponent->initializeEntity();*/

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

		/*Entity* debugRay = EntityManager::Instance->createNewEntity();
		debugRay->addToWorld(baseWorld);
		transform.setPosition(vector3D(0, 20, 0));
		debugRay->setTransform(transform);
		debugRay->m_renderer = new RayTracingDebug();*/


		if (true)
		{
			EntityWorld* BigShip = EntityManager::Instance->createNewEntityWorld();
			BigShip->addComponent("BigShip", new Component());

			BigShip->addToWorld(baseWorld);
			transform.setPosition(vector3D(0, 10, 1000));
			BigShip->setTransform(transform);

			Model* OutsideModel = new Model();
			OutsideModel->localOffset = Transform();
			OutsideModel->shader = bigCubeModel->shader;
			OutsideModel->mesh = loadMaterialMeshFromFile("res/ShipParts/", "Ship_Outside.obj");
			BigShip->tempModels.push_back(OutsideModel);

			MeshCollisionShape* OutsideShape = new MeshCollisionShape("res/ShipParts/", "Ship_Outside.obj");
			BigShip->addRigidBody(new RigidBody(OutsideShape->getCollisionShape(), 24000.0));
			//BigShip->setDampening(0.5, 0.5);
			delete OutsideShape;

			BigShip->setLinearVelocity(vector3D(0, 0, 100.0));
		}

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
			Transform transform = cam_Entity->getRenderTransform();

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