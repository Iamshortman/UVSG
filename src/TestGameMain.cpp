#include "Camera.hpp"
#include "Window.hpp"
#include "Shader/ShaderProgram.hpp"
#include "openGL.hpp"
#include "glmInclude.hpp"

#include "Mesh.hpp"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include "btBulletDynamicsCommon.h"

#include "InputButton.hpp"
#include "Transform.hpp"

using namespace std;


std::ostream &operator<< (std::ostream &out, const vector3 &vec)
{
    out << "{"
        << vec.x << ", " << vec.y << ", "<< vec.z
        << "}";

    return out;
}

std::ostream &operator<< (std::ostream &out, const quat &vec)
{
    out << "{"
        << vec.x << ", " << vec.y << ", "<< vec.z << ", " << vec.w
        << "}";

    return out;
}

int main()
{
	string Title = "Test Game";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	Window testWindow(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
    testWindow.setBufferClearColor(0.0, 0.0, 0.0, 1.0);
    InputButton input = InputButton();
    Camera camera = Camera();
    camera.moveCameraPos(vector3(0.0f ,0.0f, -10.0f));

    int num_joy = SDL_NumJoysticks();
    printf("%i joysticks were found.\n\n", num_joy);
    for(int i = 0; i < num_joy; i++)
    {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);
        printf("%s\n", SDL_JoystickName(joystick));
    }

    ShaderProgram program("basicVertex.vs", "basicFragment.fs");

    //START DEMO CODE****************************************************************************************

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(program.ShaderProgramID, "Matrix");

    //END DEMO CODE******************************************************************************************

    float time = 0;

    Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 frames = 0;

    Transform modelTransform = Transform();
    modelTransform.setPosition(0.0F, 0.0F, 0.0F);

    bool mouseCaptured = false;

	bool shouldEnd = false;
	while(shouldEnd == false)
	{

        int height, width;
        testWindow.getWindowSize(width, height);

        //FPS counter stuff
        currentTime = SDL_GetTicks();
        frames++;
        if(currentTime - lastTime > 1000)
        {
            cout << "FPS: " << frames << endl;
            lastTime = currentTime;
            frames = 0;
        }

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
            //mouseCaptured = true;
        }

        if(mouseCaptured)
        {
            testWindow.setMousePos(width/2, height/2);
        }

        testWindow.clearBuffer();
        program.setActiveProgram();

        time += 0.1F;
        //Render in here



        //End Render
        testWindow.updateBuffer();

        }

	testWindow.closeWindow();

	return 0;
}
