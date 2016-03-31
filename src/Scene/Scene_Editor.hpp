#ifndef SCENE_EDITOR_HPP
#define SCENE_EDITOR_HPP

#include "Scene/Scene.hpp"
#include "World/World.hpp"
#include "Rendering/RenderingManager.hpp"
#include "Ship/ShipComponent.hpp"
#include "Ship/ShipMeshBuilder.hpp"
#include "Rendering/ObjLoader.hpp"

class Scene_Editor : public Scene
{
public:
	Scene_Editor(SDL_GameController* controllerToUse)
	{
		controller = controllerToUse;

		//Load Skybox
		skybox = new Model();
		skybox->mesh = loadMeshFromFile("res/Skybox.obj");
		skybox->texture = "res/Skybox.png";
		skybox->shader = new ShaderProgram("res/Textured.vs", "res/Textured.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });

		MaterialShader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });

		shipModelOutside = new Model();
		shipModelOutside->localOffset = Transform();
		shipModelOutside->shader = MaterialShader;
		shipModelOutside->mesh = nullptr;

		m_cursorModel = new Model();
		m_cursorModel->localOffset = Transform();
		m_cursorModel->shader = MaterialShader;
		m_cursorModel->mesh = loadMaterialMeshFromFile("res/", "Cursor.obj");

		shipComponent = new ShipComponent();

		shipComponent->addCell(vector3S(0, 0, 0), new ShipCell());
		shipComponent->addCell(vector3S(0, 0, 1), new ShipCell());
		shipComponent->addCell(vector3S(0, 0, 2), new ShipCell());
		shipComponent->addCell(vector3S(0, 0, 3), new ShipCell());
		shipComponent->addCell(vector3S(0, 0, 4), new ShipCell());
		shipComponent->addCell(vector3S(0, 0, 5), new ShipCell());
		this->shipChanged = true;
	};

	virtual ~Scene_Editor()
	{
		delete shipComponent;
		delete shipModelInside;
		delete shipModelOutside;
		delete MaterialShader;
		delete skybox;
		delete m_cursorModel;
	};

	virtual void update(double deltaTime)
	{

		const double stepTime = 0.3;

		//UP cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(0, 0, 1);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(0, 0, 1);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Down cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(0, 0, -1);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(0, 0, -1);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Left cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(1, 0, 0);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(1, 0, 0);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Right cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(-1, 0, 0);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(-1, 0, 0);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		if (true)
		{
			static int lastState = 0;
			int currentState = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
			if (currentState && !lastState)
			{
				vector3S pos = (vector3S)m_cursorPos;
				if (shipComponent->hasCell(pos))
				{
					shipComponent->removeCell(pos);
					shipChanged = true;
				}
				else
				{
					shipComponent->addCell(pos, new ShipCell());
					shipChanged = true;
				}
			}
			lastState = currentState;
		}


		//Camera Update
		int deadzone = 4000;

		int pitchAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

		if (pitchAxis > deadzone || pitchAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)pitchAxis) / 32767.0f;
			double angle = amount * deltaTime * rotSpeed;

			//Negitive angle because the joystick layout is backwards
			quaternionD pitchQuat = glm::normalize(glm::angleAxis(-angle, cameraRot * vector3D(-1.0f, 0.0f, 0.0f)));

			cameraRot = pitchQuat * cameraRot;
		}

		int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

		if (yawAxis > deadzone || yawAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)yawAxis) / 32767.0f;
			double angle = amount * deltaTime * rotSpeed;

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

			cameraDistance += amount * deltaTime * zoomSpeed;

			if (cameraDistance < minDistance)
			{
				cameraDistance = minDistance;
			}

			if (cameraDistance > maxDistance)
			{
				cameraDistance = maxDistance;
			}
		}

		if (shipChanged == true)
		{
			if (shipModelOutside->mesh != nullptr)
			{
				delete shipModelOutside->mesh;
			}

			//if (shipMeshInside != nullptr)
			//{
			//	delete shipMeshInside;
			//}

			shipModelOutside->mesh = genOutsideMesh(shipComponent, 1.0f);

			shipChanged = false;
		}
	};

	virtual void render(RenderingManager* manager)
	{
		Camera* camera = &manager->camera;

		int width, height;
		manager->window->getWindowSize(width, height);
		camera->setProjection(45.0f, 0.01f, 1000.0f, width, height);

		vector3D camPos = (cameraRot * vector3D(0, 0, -1.0f)) * cameraDistance;
		camPos += camOrigin;

		manager->camera.setCameraTransform(camPos, cameraRot);

		Transform transform = Transform();

		/******************************************************************************************/
		manager->window->clearBuffer();

		RenderModel(camera, skybox, Transform(camera->getPos()));

		//Clear depth buffer so any other object in front of far objects.
		glClear(GL_DEPTH_BUFFER_BIT);


		if (shipModelOutside->mesh != nullptr)
		{
			RenderModel(camera, shipModelOutside, Transform());
		}

		//Cursor Draw
		glDisable(GL_DEPTH_TEST);
		RenderModel(camera, m_cursorModel, Transform(m_cursorPos));
		glEnable(GL_DEPTH_TEST);

		manager->window->updateBuffer();
	};

	ShipComponent* shipComponent = nullptr;
	ShaderProgram* MaterialShader = nullptr;
	Model* shipModelOutside = nullptr;
	Model* shipModelInside = nullptr;
	Model* skybox = nullptr;

	bool shipChanged = false;
	SDL_GameController* controller;
	vector3D camOrigin = vector3D(0.0);

	double cameraDistance = 20.0;
	double maxDistance = 200.0;
	double minDistance = 10.0;

	double rotSpeed = 3.0;
	double zoomSpeed = 15.0;

	quaternionD cameraRot;

	Model* m_cursorModel = nullptr;
	vector3D m_cursorPos = vector3D(0.0); 

private:
	void RenderModel(Camera* camera, Model* model, Transform transform)
	{
		matrix4 projectionMatrix = camera->getProjectionMatrix();
		matrix4 viewMatrix = camera->getOriginViewMatrix();

		matrix4 modelMatrix = transform.getModleMatrix(camera->getPos());
		matrix3 normalMatrix = transform.getNormalMatrix();

		modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
		normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();

		model->shader->setActiveProgram();

		model->shader->setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
		model->shader->setUniform("normalMatrix", normalMatrix);
		model->shader->setUniform("ambientLight", vector3F(1.0));

		model->mesh->draw(model->shader);

		model->shader->deactivateProgram();
	}
};

#endif //SCENE_EDITOR_HPP