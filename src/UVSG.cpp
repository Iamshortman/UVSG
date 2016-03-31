#include "UVSG.hpp"
#include "Scene/Scene_Game.hpp"
#include "Scene/Scene_Editor.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(1);

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
}

void UVSG::update(double deltaTime)
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
				SDL_GameControllerOpen(i);
			}

			continue;
		}

		if (event.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			continue;
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
