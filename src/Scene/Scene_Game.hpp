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
#include "Components/SeatComponent.hpp"

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
		bigCubeModel->shader = new ShaderProgram("res/Shaders/Material.Deferred.vs", "res/Shaders/Material.Deferred.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
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
		transform.setPosition(vector3D(-20, 20, 10));
		debugRay->setTransform(transform);
		debugRay->m_renderer = new RayTracingDebug();*/


		if (true)
		{
			EntityWorld* BigShip = EntityManager::Instance->createNewEntityWorld();
			BigShip->addComponent("BigShip", new Component());
			BigShip->addComponent("FlightControl", new ShipFlightControl());
			BigShip->setGravity(vector3D(0, -9.8, 0));

			BigShip->addToWorld(baseWorld);
			transform.setPosition(vector3D(-10, 10, 10));
			BigShip->setTransform(transform);

			Model* OutsideModel = new Model();
			OutsideModel->localOffset = Transform();
			OutsideModel->shader = bigCubeModel->shader;
			OutsideModel->mesh = loadMaterialMeshFromFile("res/Ships/Arrowhead_L/", "Arrowhead_Outside_Mesh.obj");
			BigShip->tempModels.push_back(OutsideModel);

			MeshCollisionShape* OutsideShape = new MeshCollisionShape("res/Ships/Arrowhead_L/", "Arrowhead_Outside_Mesh.obj");
			BigShip->addRigidBody(new RigidBody(OutsideShape->getCollisionShape(), 24000.0));
			//BigShip->setDampening(0.5, 0.5);
			delete OutsideShape;

			Entity* BigShipInterior = EntityManager::Instance->createNewEntity();
			BigShipInterior->addToWorld(BigShip);

			Model* InsideModel = new Model();
			InsideModel->localOffset = Transform();
			InsideModel->shader = bigCubeModel->shader;
			InsideModel->mesh = loadMaterialMeshFromFile("res/Ships/Arrowhead_L/", "Arrowhead_Inside_Mesh.obj");
			BigShipInterior->tempModels.push_back(InsideModel);

			btTriangleMesh* triMesh = new btTriangleMesh();
			loadTriMesh("res/Ships/Arrowhead_L/", "Arrowhead_Inside_Mesh.obj", triMesh);
			BigShipInterior->addRigidBody(new RigidBody(new btBvhTriangleMeshShape(triMesh, true), 0.0));


			Entity* Chair = EntityManager::Instance->createNewEntity();
			Chair->addToWorld(BigShip);

			Model* ChairModel = new Model();
			ChairModel->localOffset = Transform();
			ChairModel->shader = bigCubeModel->shader;
			ChairModel->mesh = loadMaterialMeshFromFile("res/Ships/", "Chair.obj");
			Chair->tempModels.push_back(ChairModel);

			triMesh = new btTriangleMesh();
			loadTriMesh("res/Ships/", "Chair.obj", triMesh);
			Chair->addRigidBody(new RigidBody(new btBvhTriangleMeshShape(triMesh, true), 0.0));

			Chair->setTransform(Transform(vector3D(0.0, -0.75, 2.0)));// , quaternionD(0.963, -0.164, -0.202, -0.067)));

			Chair->addComponent("Mount", new SeatComponent(Transform(vector3D(0.0, 1.0, 0.0))));

			((ShipFlightControl*)BigShip->getComponent("FlightControl"))->tempSeat = (SeatComponent*)Chair->getComponent("Mount");
		}


		//SmallShip Creation
		if (true)
		{
			Entity* smallShip = EntityManager::Instance->createNewEntity();
			smallShip->addComponent("FlightControl", new ShipFlightControl());

			smallShip->addToWorld(baseWorld);
			transform.setPosition(vector3D(0, 10, 10));
			smallShip->setTransform(transform);

			Model* OutsideModel = new Model();
			OutsideModel->localOffset = Transform();
			OutsideModel->shader = bigCubeModel->shader;
			OutsideModel->mesh = loadMaterialMeshFromFile("res/Ships/Arrowhead_S/", "Outside_Body_Canopy.obj");
			smallShip->tempModels.push_back(OutsideModel);

			Model* InsideModel = new Model();
			InsideModel->localOffset = Transform();
			InsideModel->shader = bigCubeModel->shader;
			InsideModel->mesh = loadMaterialMeshFromFile("res/Ships/Arrowhead_S/", "Inside_Body_Canopy.obj");
			smallShip->tempTransparentModels.push_back(InsideModel);

			Model* LaserGun = new Model();
			LaserGun->localOffset = Transform(vector3D(0.5, -0.25, 1.75));
			LaserGun->shader = bigCubeModel->shader;
			LaserGun->mesh = loadMaterialMeshFromFile("res/Ships/", "LaserCannon.obj");
			smallShip->tempModels.push_back(LaserGun);

			Model* LaserGun1 = new Model();
			LaserGun1->localOffset = Transform(vector3D(-0.5, -0.25, 1.75));
			LaserGun1->shader = bigCubeModel->shader;
			LaserGun1->mesh = LaserGun->mesh;
			smallShip->tempModels.push_back(LaserGun1);

			//CollsionShapes
			btCompoundShape* shape = new btCompoundShape();

			MeshCollisionShape* OutsideShape = new MeshCollisionShape("res/Ships/Arrowhead_S/", "Collision_Body.obj");
			shape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0, 0.0, 0.0)), OutsideShape->getCollisionShape());
			delete OutsideShape;
			OutsideShape = new MeshCollisionShape("res/Ships/Arrowhead_S/", "Collision_Canopy.obj");
			shape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0, 0.0, 0.0)), OutsideShape->getCollisionShape());
			delete OutsideShape;
			smallShip->addRigidBody(new RigidBody(shape, 2400.0));

			//Seat
			smallShip->addComponent("Mount", new SeatComponent(Transform(vector3D(0.0, 0.9125, 0.0))));
			((ShipFlightControl*)smallShip->getComponent("FlightControl"))->tempSeat = (SeatComponent*)smallShip->getComponent("Mount");
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
		manager->window->clearBuffer();

		manager->RenderMainWorld(0.0, baseWorld);

		/*if (cam_Entity->getWorld() != baseWorld)
		{
			manager->RenderSecondaryWorld(0.0, cam_Entity->getWorld());
		}*/

		manager->window->updateBuffer();

	};

	World* baseWorld;
};

#endif //SCENE_GAME_HPP