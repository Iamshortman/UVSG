#include "UVSG.hpp"
#include "World/EntityManager.hpp"
#include "Rendering/EntityRenderer.hpp"
#include "Rendering/ObjLoader.hpp"

#include "Ship/ShipMeshBuilder.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(1);

	m_renderer = new EntityRenderer();

	Transform camTransform;
	camTransform.setPos(vector3D(-10.0f, 10.0f, -10.0f));
	camTransform.m_orientation = glm::angleAxis(toRad(45.0), vector3D(1, 0, 0)) * camTransform.m_orientation;
	camTransform.m_orientation = glm::angleAxis(toRad(45.0), vector3D(0, 1, 0)) * camTransform.m_orientation;
	this->renderingManager->camera.setCameraTransform(camTransform.getPos(), camTransform.getOrientation());

	Entity* entity = EntityManager::instance()->createNewEntity();
	entity->addToWorld(baseWorld);

	ShipComponent* ship = new ShipComponent();
	ship->addCell(vector3S(0, 0, 0), new ShipCell());
	ship->addCell(vector3S(0, 0, 1), new ShipCell());
	ship->addCell(vector3S(0, 0, 2), new ShipCell());

	ship->getCell(vector3S(0, 0, 0))->parts[5] = new ShipPart();

	Model* model1 = new Model();
	model1->localOffset = Transform();
	model1->shader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
	model1->mesh = genOutsideMesh(ship);

	Model* model2 = new Model();
	model2->localOffset = Transform(vector3D(0, 0, 0), glm::angleAxis(toRad(-90.0), vector3D(0, 1, 0)));

	model2->shader = model1->shader;
	model2->mesh = loadMaterialMeshFromFile("res/ShipParts/", "TMP_Window.obj");

	CusorModel = new Model();
	CusorModel->mesh = loadMaterialMeshFromFile("res/Models/", "CubeIcon.obj");
	CusorModel->shader = model1->shader;

	entity->tempModels.push_back(model1);
	entity->tempModels.push_back(model2);
}

void UVSG::update(double timeStep)
{
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

			if (SDL_IsGameController(i))
			{
				controller = SDL_GameControllerOpen(i);
			}

			continue;
		}

		if (event.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			continue;
		}

		renderingManager->window->HandleEvent(event);

	}


	//ShipEditor Update
	int deadzone = 8000;

	int pitchAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

	if (pitchAxis > deadzone || pitchAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)pitchAxis) / 32767.0f;
		double angle = amount * timeStep * 3.0f;

		//Negitive angle because the joystick layout is backwards
		quaternionD pitchQuat = glm::normalize(glm::angleAxis(-angle, cameraRot * vector3D(-1.0f, 0.0f, 0.0f)));

		cameraRot = pitchQuat * cameraRot;
	}

	int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

	if (yawAxis > deadzone || yawAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)yawAxis) / 32767.0f;
		double angle = amount * timeStep * 3.0f;

		quaternionD yawQuat = glm::normalize(glm::angleAxis(-angle, vector3D(0.0f, 1.0f, 0.0f)));

		cameraRot = yawQuat * cameraRot;
	}

	int rightTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	int leftTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

	if (rightTrigger > deadzone || leftTrigger > deadzone)
	{
		int inputValue = -rightTrigger + leftTrigger;
		//Get between -1 and 1
		double amount = ((double)inputValue) / 32767.0f;

		cameraDistance += amount * timeStep * 15.0;
		if (cameraDistance < 5.0)
		{
			cameraDistance = 5.0;
		}

		if (cameraDistance > 100.0)
		{
			cameraDistance = 100.0;
		}
	}


	vector3D camPos = (cameraRot * vector3D(0, 0, -1.0f)) * cameraDistance;
	camPos += camOrigin;

	renderingManager->camera.setCameraTransform(camPos, cameraRot);


	//baseWorld->updateWorld(timeStep);

	//#5 rendering
	renderingManager->window->set3dRendering();
	renderingManager->update(timeStep, baseWorld);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Model* model = CusorModel;

	matrix4 projectionMatrix = renderingManager->camera.getProjectionMatrix();
	matrix4 viewMatrix = renderingManager->camera.getOriginViewMatrix();

	Transform transform = Transform(vector3D(0, 0, -2.5));
	matrix4 modelMatrix = transform.getModleMatrix(renderingManager->camera.getPos());
	matrix3 normalMatrix = transform.getNormalMatrix();

	//modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
	//normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();

	model->shader->setActiveProgram();

	model->shader->setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
	model->shader->setUniform("normalMatrix", normalMatrix);
	model->shader->setUniform("ambientLight", vector3F(1.0));

	model->mesh->draw(model->shader);

	model->shader->deactivateProgram();

	glDisable(GL_BLEND);

	renderingManager->window->updateBuffer();

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
	delete renderingManager;
	delete baseWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}
