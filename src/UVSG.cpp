#include "UVSG.hpp"
#include "Input/InputManager.hpp"
#include "Scene/Scene_Game.hpp"
#include "Scene/Scene_Editor.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(1);

	if (true)
	{
		currentScene = new Scene_Game();
	}
	else
	{
		currentScene = new Scene_Editor();
	}

	shipCellDictionary = new ShipCellDictionary();
	//shipCellDictionary->loadAllCellsFromFolder("res/ShipParts/Small_Ship/");
	//shipCellDictionary->loadAllCellsFromFolder("res/ShipParts/Big_Ship/");
}

UVSG::~UVSG()
{
	delete currentScene;
	delete renderingManager;
	delete shipCellDictionary;
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
				SDL_GameController* controller = SDL_GameControllerOpen(i);
				InputManager::Instance->loadController(controller);
			}

			continue;
		}

		if (event.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			int i = event.cbutton.which;
			
			if (SDL_IsGameController(i))
			{
				SDL_GameController* controller = SDL_GameControllerFromInstanceID(i);
				InputManager::Instance->unloadController(controller);

			}

			continue;
		}

		if (event.type == SDL_MOUSEWHEEL)
		{
			((Scene_Editor*)currentScene)->intMouse = event.wheel.y;
		}

		renderingManager->window->HandleEvent(event);

	}
	
	InputManager::Instance->update(deltaTime);

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

UVSG* UVSG::getInstance()
{
	return instance;
}
