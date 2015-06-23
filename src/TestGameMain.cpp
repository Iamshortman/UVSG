#include "Camera.hpp"
#include "Window.hpp"
#include "ShaderProgram.hpp"
#include "openGL.hpp"
#include "glmInclude.hpp"
#include "Mesh.hpp"

#include "UVSG.hpp"


#include <SDL2/SDL.h>
#undef main //Not sure if this is needed

#include <stdlib.h>
#include <math.h>

#include "InputButton.hpp"

using namespace std;

int main()
{
	UVSG::getInstance();


	string Title = "UVSG";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	Window testWindow(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
    testWindow.setBufferClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    InputButton input = InputButton();

    Camera camera = Camera();
    camera.moveCameraPos(btVector3(0.0, 10.0F, 25.0F));

    int num_joy = SDL_NumJoysticks();
    printf("%i joysticks were found.\n\n", num_joy);
    for(int i = 0; i < num_joy; i++)
    {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);
        printf("%s\n", SDL_JoystickName(joystick));
    }

    AttributeLocation attributes[] = { {0, "in_Position"}, {1, "in_Color"} };

    ShaderProgram program("basicVertex.vs", "basicFragment.fs", attributes, 2);

	// Get a handle for our "Matrix" uniform
	GLuint MatrixID = glGetUniformLocation(program.programID, "MVP");
	GLuint ModelID = glGetUniformLocation(program.programID, "ModelMatrix");
	GLuint NormalID = glGetUniformLocation(program.programID, "NormalMatrix");

    float deltaTime = 0;

    Uint32 lastTime = 0;
    Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 frames = 0;

    bool mouseCaptured = false;

	bool shouldEnd = false;
	while(shouldEnd == false)
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

		deltaTime = ((float)delta) / 1000.0f;

        SDL_Event event;
        while( SDL_PollEvent( &event ) )
        {
            if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                shouldEnd = true;
            }

            input.HandleEvent(event);
            testWindow.HandleEvent(event);
        }

        if(input.isKeyboardButtonDown(SDL_SCANCODE_ESCAPE) || !testWindow.isWindowActive())
        {
            SDL_ShowCursor(SDL_ENABLE);
            mouseCaptured = false;
        }

        if(input.isMouseButtonDown(SDL_BUTTON_LEFT))
        {
                //SDL_ShowCursor(SDL_DISABLE);
                mouseCaptured = true;
        }


		UVSG::getInstance()->update(deltaTime);

        testWindow.clearBuffer();
        program.setActiveProgram();

        //End Render
		testWindow.updateBuffer();

	}


	testWindow.closeWindow();

	return 0;
}
