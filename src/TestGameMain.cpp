#include "Rendering/Camera.hpp"
#include "Rendering/Window.hpp"
#include "Rendering/ShaderProgram.hpp"
#include "Rendering/openGL.hpp"
#include "glmInclude.hpp"
#include "Rendering/Mesh.hpp"

#include "UVSG.hpp"

#include <SDL2/SDL.h>
#undef main //Not sure if this is needed

#include <stdlib.h>
#include <math.h>

using namespace std;

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//Disable Events for controllers cause they suck.
	SDL_GameControllerEventState(SDL_IGNORE);

	UVSG* game = new UVSG();

    int num_joy = SDL_NumJoysticks();
    printf("\n%i joystick(s) found.\n\n", num_joy);
    for(int i = 0; i < num_joy; i++)
    {
		game->joystick = SDL_JoystickOpen(i);
		if (SDL_IsGameController(i))
		{
			game->controller = SDL_GameControllerOpen(i);
			printf("%s \n", SDL_JoystickName(game->joystick));
		}
    }

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
