
#include <SDL2/SDL.h>
#undef main //Not sure if this is needed

#include "World/EntityManager.hpp"
#include "UVSG.hpp"

#include <stdlib.h>
#include <math.h>

using namespace std;

int main()
{

	//Creates the entity Manager.
	EntityManager::createInstance();

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GameControllerAddMappingsFromFile("res/gamecontrollerdb.txt");

	UVSG* game = new UVSG();

    double deltaTime = 0;

    Uint32 lastTime = 0;
    Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 frames = 0;

    bool mouseCaptured = false;

	while(!UVSG::getInstance()->getShouldClose())
	{
        //FPS counter stuff
        currentTime = SDL_GetTicks();
        frames++;
        if(currentTime - lastTime > 1000)
        {
            cout << "FPS: " << frames << endl;
            lastTime = currentTime;
            frames = 0;
        }
        Uint32 delta = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

		deltaTime = ((double)delta) / 1000.0;
		//cout << "Update delta: " << deltaTime << endl;

		UVSG::getInstance()->update(deltaTime);
	}

	delete game;

	//Requires the enter button to be hit before we end.
	cin.get();
	

	return 0;
}
