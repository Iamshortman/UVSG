#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "Components.hpp"
#include "Renderable.hpp"

#include "Physics/RigidBody.hpp"

#include "Rendering/ObjLoader.hpp"
#include "Components/PlayerControl.hpp"
#include "Components/CharacterController.hpp"
#include "Components/TimeToLive.hpp"

#include "Rendering/ColoredMesh.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(1);
	this->physicsWorld = new PhysicsWorld();

	entitySystem.systems.add<PlayerControlSystem>();
	entitySystem.systems.add<CharacterControllerSystem>();
	entitySystem.systems.add<TimeToLiveSystem>();
	entitySystem.systems.configure();

	Transform camTransform;
	camTransform.setPos(vector3D(-5.0f, 5.0f, -5.0f));
	camTransform.m_orientation = glm::angleAxis(toRad(30.0), vector3D(1, 0, 0)) * camTransform.m_orientation;
	camTransform.m_orientation = glm::angleAxis(toRad(45.0), vector3D(0, 1, 0)) * camTransform.m_orientation;
	this->renderingManager->camera.setCameraTransform(camTransform.getPos(), camTransform.getOrientation());

	m_camera = entitySystem.entities.create();
	m_camera.assign<Transform>();
	m_camera.assign<PlayerControlComponent>(5.0, 3.0);
	m_camera.component<Transform>()->setTransform(camTransform);
	m_camera.assign<NearZoneRenderable>();

	Entity star = entitySystem.entities.create();
	star.assign<FarZoneRenderable>();
	star.assign<Transform>();
	star.component<Transform>()->setPos(vector3D(200000.0, 0.0, 200000.0));
	star.component<Transform>()->setScale(vector3D(60000.0));

	Model* model = new Model();
	model->shader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
	model->mesh = loadMaterialMeshFromFile("res/Models/", "Sphere.obj");
	star.component<FarZoneRenderable>()->models.push_back(model);

	player = entitySystem.entities.create();
	player.assign<Transform>();
	player.assign<Velocity>();
	player.assign<CharacterController>(4.0, 8.0, 4.0, 0.5);
	player.assign<NearZoneRenderable>();
	player.assign<RigidBody>(physicsWorld, new btCapsuleShape(0.4, 1.1), 1.0, btVector3(0, 0, 0));
	Model* model1 = new Model();
	model1->shader = model->shader;
	//model1->mesh = loadMaterialMeshFromFile("res/Models/", "LaserGun.obj");
	//player.component<NearZoneRenderable>()->models.push_back(model1);
	player.component<Transform>()->setPos(vector3D(-10.0, 10.0, 5.0));

	Entity ground = entitySystem.entities.create();
	ground.assign<Transform>();
	ground.assign<NearZoneRenderable>();
	Model* model2 = new Model();
	model2->shader = model->shader;
	model2->mesh = loadMaterialMeshFromFile("res/Models/", "Level.obj");
	ground.component<NearZoneRenderable>()->models.push_back(model2);

	btTriangleMesh* triMesh = new btTriangleMesh();
	loadTriMesh("res/Models/", "Level.obj", triMesh);

	ground.assign<RigidBody>(physicsWorld, new btBvhTriangleMeshShape(triMesh, true), 0.0);

	//*************************************************************************
    std::vector<ColoredVertex> vertices;
    vertices.push_back({vector3F(1, 1, 0), vector3F(0.0f), vector3F(0.0f)});
	vertices.push_back({vector3F(-1, 1, 0), vector3F(0.0f), vector3F(0.0f)});
	vertices.push_back({vector3F(-1, -1, 0), vector3F(0.0f), vector3F(0.0f)});
	vertices.push_back({vector3F(1, -1, 0), vector3F(0.0f), vector3F(0.0f)});

    std::vector<unsigned int> indices;
	//Triangle 1
	indices.push_back(1); indices.push_back(2); indices.push_back(0);

	//Triangle 2
	indices.push_back(2); indices.push_back(3); indices.push_back(0);

    Billboard* board = new Billboard();
    board->mesh = new ColoredMesh(vertices, indices);
    board->scale = vector3F(1.5f);
    board->color = vector3F(220, 20, 60) / 255.0f;
    board->shader = new ShaderProgram("res/ColoredBillboardVertex.vs", "res/ColoredBillboardFragment.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Color" } });
    star.component<FarZoneRenderable>()->billboards.push_back(board);

	Model* gunModel = new Model();
	gunModel->mesh = loadMaterialMeshFromFile("res/Models/", "0x10c_Rife.obj");
	gunModel->shader = model->shader;
	renderingManager->firstPersonModel = gunModel;

	boltModel = new Model();
	boltModel->mesh = loadMaterialMeshFromFile("res/Models/", "PlasmaBolt.obj");
	boltModel->shader = model->shader;
}

void UVSG::update(double timeStep)
{
	entitySystem.systems.update_all(timeStep);

	ComponentHandle<Transform> transform = player.component<Transform>();
	ComponentHandle<CharacterController> character = player.component<CharacterController>();

	this->renderingManager->camera.setCameraTransform(transform->getPos() + vector3D(0, 0.6, 0), glm::normalize(glm::angleAxis(character->tempCameraPitch, transform->getRight()) * transform->getOrientation()));

	static int lastButton = 0;
	int button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
	if (button && !lastButton)
	{
		quaternionD rot = glm::normalize(glm::angleAxis(character->tempCameraPitch, transform->getRight()) * transform->getOrientation());
		vector3D pos = transform->getPos() + vector3D(0, 0.6, 0);
		vector3D offset = rot * vector3D(-0.6, -.4, 0);

		tempCreatePlasmaBolt(pos + offset, rot);
	}
	lastButton = button;

	ComponentHandle<Bolt> boltSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(boltSearch))
	{
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();
		ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();

		componentTransform->m_position += componentVelocity->linearVelocity * timeStep;

		double rayDistance = 0.5f;
		vector3D startPos = componentTransform->getPos();
		vector3D endPos = componentTransform->getPos() + (componentTransform->getForward() * rayDistance);
		SingleRayTestResult result = UVSG::getInstance()->physicsWorld->singleRayTest(startPos, endPos);

		if (result.hasHit)
		{
			entity.destroy();
		}
	}


	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
		}

		if (event.type == SDL_CONTROLLERDEVICEADDED)
		{
			int i = event.cdevice.which;

			joystick = SDL_JoystickOpen(i);
			if (SDL_IsGameController(i))
			{
				controller = SDL_GameControllerOpen(i);
				printf("%s \n", SDL_JoystickName(joystick));
			}

			continue;
		}

		if (event.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			continue;
		}

		renderingManager->window->HandleEvent(event);

	}

	//#1 step Physics
	physicsWorld->update(entitySystem, timeStep);

	//#2 update Input
	//Update Key Bindings

	//#3 game logic updates


	//#4 audio update

	//#5 rendering
	renderingManager->window->set3dRendering();
	renderingManager->update(entitySystem, timeStep);

	//#6 UI Rendering
	renderingManager->window->set2dRendering();

}

void UVSG::exitGame()
{
	shouldClose = true;
}

const bool UVSG::getShouldClose()
{
	return shouldClose;
}

UVSG::~UVSG()
{
	for (Entity entity : entitySystem.entities.entities_for_debugging())
	{
		entity.destroy();
	}

	delete renderingManager;
	delete physicsWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}

void UVSG::tempCreatePlasmaBolt(vector3D pos, quaternionD direction)
{
	Entity bolt = entitySystem.entities.create();
	bolt.assign<Transform>(pos, direction);
	bolt.assign<Velocity>();
	bolt.component<Velocity>()->linearVelocity = direction * vector3D(0, 0, 1);
	bolt.component<Velocity>()->linearVelocity *= 30;
	bolt.assign<NearZoneRenderable>();
	bolt.component<NearZoneRenderable>()->models.push_back(boltModel);
	bolt.assign<Bolt>();
	bolt.assign<TimeToLiveComponent>(20.0);
}