#ifndef SCENE_EDITOR_HPP
#define SCENE_EDITOR_HPP

#include "Scene/Scene.hpp"
#include "World/EntityManager.hpp"
#include "World/World.hpp"
#include "Rendering/RenderingManager.hpp"
#include "Rendering/ObjLoader.hpp"
#include "Renderer/ShipRenderer.hpp"
#include "Gui/Gui.hpp"

#include "Ship/ShipComponent.hpp"
#include "Ship/ShipCell.hpp"

#include "Components/ShipFlightControl.hpp"

class Scene_Editor : public Scene
{
public:
	Gui* m_Gui;

	int shipCell_Index = 0;

	//Temp int Mouse
	int intMouse = 0;

	Scene_Editor()
	{
		m_Gui = new Gui();

		//Load Skybox
		skybox = new Model();
		skybox->mesh = loadMeshFromFile("res/Skybox.obj");
		skybox->texture = "res/Skybox.png";
		skybox->shader = new ShaderProgram("res/Textured.vs", "res/Textured.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });

		MaterialShader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });

		directionalLight = new DirectionalLight(vector3F(1.0f, -1.0f, -1.0f), vector3F(1.0f, 1.0f, 1.0f), 0.6f);
		DirectionalShader = new ShaderProgram("res/Material.vs", "res/foward-directional.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
		pointLight = new PointLight(vector3D(0.0, 10.0, 0.0), 20.0f, vector3F(1.0), 0.4f, vector3F(0.0f, 0.0f, 0.02f));
		PointShader = new ShaderProgram("res/Material.vs", "res/foward-point.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });

		shipModelOutside = new Model();
		shipModelOutside->localOffset = Transform();
		shipModelOutside->shader = MaterialShader;
		shipModelOutside->mesh = nullptr;
		shipModelOutside->texture = "";

		m_cursorModel = new Model();
		m_cursorModel->localOffset = Transform();
		m_cursorModel->shader = MaterialShader;
		m_cursorModel->mesh = loadMaterialMeshFromFile("res/", "Cursor.obj");
		m_cursorModel->texture = "";

		shipComponent = new ShipComponent(cubeSizeOutside);
		this->shipChanged = true;

		UVSG::getInstance()->renderingManager->texturePool.loadTexture("res/Textures/metalPanel_Cells_Green.png");
		UVSG::getInstance()->renderingManager->texturePool.loadTexture("res/Textures/glassPanel.png");
	};

	virtual ~Scene_Editor()
	{
		delete shipModelInside;
		delete shipModelOutside;

		delete MaterialShader;

		delete skybox->mesh;
		delete skybox->shader;

		delete m_cursorModel;

		delete directionalLight;
		delete DirectionalShader;
		delete pointLight;
		delete PointShader;
	};

	virtual void update(double deltaTime)
	{
		vector<ShipCell*> shipCells = UVSG::getInstance()->shipCellDictionary->getCategory("smallship_all");

		const double stepTime = 0.3;

		double fowardBack = InputManager::Instance->getAxis("editor_cell_forward_back");
		double LeftRight = InputManager::Instance->getAxis("editor_cell_left_right");

		//FORWARD cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (fowardBack > 0.0)
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

		//BACK cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (fowardBack < 0.0)
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
			if (LeftRight > 0.0)
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
			if (LeftRight < 0.0)
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

		//Up cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (InputManager::Instance->getButtonDown("editor_cell_up"))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(0, 1, 0);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(0, 1, 0);
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
			if (InputManager::Instance->getButtonDown("editor_cell_down"))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(0, -1, 0);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(0, -1, 0);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Camera Update

		if (InputManager::Instance->hasAxis("editor_camera_pitch"))
		{
			//Get between -1 and 1
			double amount = InputManager::Instance->getAxis("editor_camera_pitch");
			double angle = amount * deltaTime * rotSpeed;

			quaternionD pitchQuat = glm::normalize(glm::angleAxis(-angle, cameraRot * vector3D(-1.0, 0.0, 0.0)));

			cameraRot = pitchQuat * cameraRot;
		}

		if (InputManager::Instance->hasAxis("editor_camera_yaw"))
		{
			//Get between -1 and 1
			double amount = InputManager::Instance->getAxis("editor_camera_yaw");
			double angle = amount * deltaTime * rotSpeed;

			quaternionD yawQuat = glm::normalize(glm::angleAxis(-angle, vector3D(0.0, 1.0, 0.0)));

			cameraRot = yawQuat * cameraRot;
		}

		/*if (true)
		{
			int inputValue = -rightTrigger + leftTrigger;
			//Get between -1 and 1
			double amount = ((double)inputValue) / 32767.0;

			cameraDistance += amount * deltaTime * zoomSpeed;

			if (cameraDistance < minDistance)
			{
				cameraDistance = minDistance;
			}

			if (cameraDistance > maxDistance)
			{
				cameraDistance = maxDistance;
			}
		}*/

		//MOUSE CONTROLLS
		static int last_MousePos[] = { 0, 0 };
		int current_MousePos[] = { 0, 0 };
		if (SDL_GetMouseState(&current_MousePos[0], &current_MousePos[1]) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			int delta_Mouse[] = { last_MousePos[0] - current_MousePos[0], last_MousePos[1] - current_MousePos[1] };

			if (true)
			{
				//Get between -1 and 1
				double amount = delta_Mouse[1];
				double angle = amount * deltaTime * 0.1;

				quaternionD pitchQuat = glm::normalize(glm::angleAxis(angle, cameraRot * vector3D(-1.0f, 0.0f, 0.0f)));

				cameraRot = pitchQuat * cameraRot;
			}

			if (true)
			{
				//Get between -1 and 1
				double amount = delta_Mouse[0];
				double angle = amount * deltaTime  * 0.1;

				quaternionD yawQuat = glm::normalize(glm::angleAxis(angle, vector3D(0.0f, 1.0f, 0.0f)));

				cameraRot = yawQuat * cameraRot;
			}
		}

		if (true)
		{
			double amount = -intMouse;

			cameraDistance += amount * deltaTime * zoomSpeed * 2.0;

			if (cameraDistance < minDistance)
			{
				cameraDistance = minDistance;
			}

			if (cameraDistance > maxDistance)
			{
				cameraDistance = maxDistance;
			}
		}

		last_MousePos[0] = current_MousePos[0];
		last_MousePos[1] = current_MousePos[1];

		intMouse = 0;

		Camera* camera = &UVSG::getInstance()->renderingManager->camera;
		vector3D camPos = (cameraRot * vector3D(0, 0, -1.0f)) * cameraDistance;
		camPos += camOrigin;
		camera->setCameraTransform(camPos, cameraRot);

		if (true)
		{
			static int lastState = 0;
			int currentState = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT);
			if (currentState && !lastState)
			{
				vector3B pos = (vector3B)m_cursorPos;
				if (shipComponent->hasCellAtPos(pos))
				{
					shipComponent->removeCell(pos);
					shipChanged = true;
				}
				else
				{
					ShipCellData newCell = ShipCellData(shipCells[shipCell_Index], (vector3B)m_cursorPos);
					if (shipComponent->canPlaceCell(newCell))
					{
						shipComponent->addCell(newCell);
						shipChanged = true;
					}
				}
			}
			lastState = currentState;
		}

		//EDITOR CONTROLS
		if (InputManager::Instance->getButtonPressed("editor_cell_place"))
		{
			vector3B pos = (vector3B)m_cursorPos;
			if (shipComponent->hasCellAtPos(pos))
			{
				shipComponent->removeCell(pos);
				shipChanged = true;
			}
			else if (shipCells[shipCell_Index] != nullptr)
			{
				ShipCellData newCell = ShipCellData(shipCells[shipCell_Index], (vector3B)m_cursorPos);
				if (shipComponent->canPlaceCell(newCell))
				{
					shipComponent->addCell(newCell);
					shipChanged = true;
				}
			}
		}

		if (InputManager::Instance->getButtonPressed("editor_cell_next"))
		{
			shipCell_Index--;
			if (shipCell_Index < 0)
			{
				shipCell_Index = (int)shipCells.size() - 1;
			}

		}

		if (InputManager::Instance->getButtonPressed("editor_cell_prev"))
		{
			shipCell_Index++;
			if (shipCell_Index >  shipCells.size() - 1)
			{
				shipCell_Index = 0;
			}
		}

		if (shipChanged == true)
		{
			if (shipModelOutside->mesh != nullptr)
			{
				delete shipModelOutside->mesh;
			}

			shipModelOutside->mesh = shipComponent->genOutsideMesh();

			shipChanged = false;
		}

		if (shipCell_Index < shipCells.size() && shipCells[shipCell_Index] != nullptr && shipComponent->canPlaceCell(ShipCellData(shipCells[shipCell_Index], (vector3B)m_cursorPos)))
		{
			MaterialMesh* mesh = (MaterialMesh*)m_cursorModel->mesh;
			mesh->materials[0].diffuse_Color = vector3F(0.0f, 1.0f, 0.0f);
		}
		else
		{
			MaterialMesh* mesh = (MaterialMesh*)m_cursorModel->mesh;
			mesh->materials[0].diffuse_Color = vector3F(0.800000f, 0.254604f, 0.002949f);
		}

		//Load game world
		if (InputManager::Instance->getButtonPressed("editor_exit"))
		{
			printf("Loading Game World!!!\n");

			Entity* ship = EntityManager::Instance->createNewEntity();

			UVSG* instace = UVSG::getInstance();
			Scene_Game* game = new Scene_Game();
			instace->currentScene = game;

			ship->m_renderer = new ShipRenderer();
			ship->addComponent("FlightControl", new ShipFlightControl());
			ship->addComponent("shipComponent", shipComponent);
			ship->addToWorld(game->baseWorld);
			shipComponent->initializeEntity();
			
			ship->setTransform(Transform(vector3D(0, 10, 0)));
		}
	};

	virtual void render(RenderingManager* manager)
	{
		manager->window->set3dRendering();

		Camera* camera = &manager->camera;

		int width, height;
		manager->window->getWindowSize(width, height);
		camera->setProjection(45.0f, 0.01f, 1000.0f, width, height);

		Transform transform = Transform();

		/******************************************************************************************/
		manager->window->clearBuffer();

		manager->renderModel(camera, skybox, Transform(camera->getPosition()));

		//Clear depth buffer so any other object in front of far objects.
		glClear(GL_DEPTH_BUFFER_BIT);

		Model* model = new Model();
		model->shader = MaterialShader;
		for (auto it = shipComponent->m_shipCells.begin(); it != shipComponent->m_shipCells.end(); ++it)
		{
			model->mesh = it->second.getMesh();
			if (model->mesh != nullptr)
			{
				model->localOffset = Transform((vector3F)it->second.m_position * cubeSizeOutside);
				manager->renderModelLight(camera, model, Transform());
			}
		}
		delete model;

		if (shipModelOutside->mesh != nullptr)
		{
			manager->renderModelLight(camera, shipModelOutside, Transform());
		}

		//Cursor Draw
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		vector<ShipCell*> shipCells = UVSG::getInstance()->shipCellDictionary->getCategory("smallship_all");
		if (shipCell_Index < shipCells.size() && shipCells[shipCell_Index] != nullptr)
		{
			Model* model = new Model();
			model->shader = MaterialShader;
			model->mesh = shipCells[shipCell_Index]->getCursorMesh();

			if (model->mesh != nullptr)
				manager->renderModelLight(camera, model, Transform((vector3D)m_cursorPos * (double)cubeSizeOutside), 0.5f);

			delete model;
		}

		glDisable(GL_DEPTH_TEST);
		manager->renderModelLight(camera, m_cursorModel, Transform((vector3D)m_cursorPos * (double)cubeSizeOutside));
		glEnable(GL_DEPTH_TEST);

		glDisable(GL_BLEND);

		//UI Rendering
		//Clear depth buffer so any other object in front of far objects.
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		manager->window->set2dRendering();
		vector2I windowSize = vector2I(width, height);

		manager->texturePool.bindTexture("res/Textures/metalPanel_Cells_Green.png");
		m_Gui->drawQuad(vector2I(0), vector2I(110, 509), windowSize);

		manager->texturePool.bindTexture("res/Textures/glassPanel.png");
		m_Gui->drawQuad(vector2I(5, 30 + (100 * shipCell_Index)), vector2I(100, 100), windowSize);

		glDisable(GL_BLEND);

		manager->window->updateBuffer();
	};

	float cubeSizeOutside = 1.0f;
	float cubeSizeInside = 2.6f;

	ShipComponent* shipComponent = nullptr;
	ShaderProgram* MaterialShader = nullptr;
	Model* shipModelOutside = nullptr;
	Model* shipModelInside = nullptr;
	Model* skybox = nullptr;
	Model* UIBox = nullptr;

	DirectionalLight* directionalLight = nullptr;
	ShaderProgram* DirectionalShader = nullptr;

	PointLight* pointLight = nullptr;
	ShaderProgram* PointShader = nullptr;

	bool shipChanged = false;

	vector3D camOrigin = vector3D(0.0);

	double cameraDistance = 20.0;
	double maxDistance = 200.0;
	double minDistance = 5.0;

	double rotSpeed = 3.0;
	double zoomSpeed = 150.0;

	quaternionD cameraRot;

	Model* m_cursorModel = nullptr;
	vector3B m_cursorPos = vector3D(0.0); 

private:
	
};

#endif //SCENE_EDITOR_HPP