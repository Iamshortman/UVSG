#include "UVSG.hpp"
#include "Scene/Scene_Game.hpp"
#include "Scene/Scene_Editor.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();

	if (SDL_IsGameController(0))
	{
		SDL_GameController* controller = SDL_GameControllerOpen(0);
		currentScene = new Scene_Editor(controller);
		printf("Using Controller: %s \n", SDL_GameControllerName(controller));
	}
	else
	{
		exit(15);
	}

	/*float width = 600.0f;
	float height = 400.0f;

	matrix4 projectionMatrix = glm::perspective(45.0f, width / height, 0.1f, 1000.0f);
	vector3F screenPos = vector3F(width / 2.0f, height / 2.0f, 1.0f);
	std::cout << "{" << screenPos.x << ", " << screenPos.y << ", " << screenPos.z << "} \n";

	vector4F viewport = vector4F(0.0f, 0.0f, width, height);
	matrix4 viewMatrix = matrix4();
	vector3F worldPos = glm::unProject(screenPos, viewMatrix, projectionMatrix, viewport);

	std::cout << "{" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << "} \n";*/
}

void UVSG::update(double deltaTime)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
		}

		if (event.type == SDL_CONTROLLERDEVICEADDED)
		{
			int i = event.cdevice.which;

			if (SDL_IsGameController(i))
			{
				SDL_GameControllerOpen(i);
			}

			continue;
		}

		if (event.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			continue;
		}

		if (event.type == SDL_MOUSEWHEEL)
		{
			((Scene_Editor*)currentScene)->intMouse = event.wheel.y;
		}

		renderingManager->window->HandleEvent(event);

	}

	currentScene->update(deltaTime);


	currentScene->render(renderingManager);

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
	delete currentScene;
	delete renderingManager;
}

UVSG* UVSG::getInstance()
{
	return instance;
}
