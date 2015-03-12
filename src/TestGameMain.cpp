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
	string Title = "UVSG";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	Window testWindow(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
    testWindow.setBufferClearColor(0.0, 0.0, 0.0, 1.0);
    InputButton input = InputButton();
    Camera camera = Camera();

    int num_joy = SDL_NumJoysticks();
    printf("%i joysticks were found.\n\n", num_joy);
    for(int i = 0; i < num_joy; i++)
    {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);
        printf("%s\n", SDL_JoystickName(joystick));
    }

    ShaderProgram program("basicVertex.vs", "basicFragment.fs");

	// Get a handle for our "Matrix" uniform
	GLuint MatrixID = glGetUniformLocation(program.ShaderProgramID, "Matrix");

    Mesh mesh = Mesh();
    const vector3 vertices[] =
    {
    vector3(-1.0f, -1.0f, -1.0f),
    vector3(1.0f, -1.0f, -1.0f),
    vector3(0.0f, 1.0f, -1.0f)
    };

    const vector3 colors[]=
    {
    vector3(1.0F, 1.0F, 0.0F),
    vector3(0.0F, 1.0F, 1.0F),
    vector3(1.0F, 0.0F, 1.0F)
    };

    const int indices[] = {0, 1, 2};

    mesh.addVertices(vertices, colors, indices, sizeof(vertices));

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

        float screenRes = ((float)width)/((float)height);
        matrix4 ProjectionMatrix = glm::perspective(45.0F, screenRes, 0.1F, 100.0F);

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

        matrix4 ViewMatrix = camera.getViewMatrix();

        matrix4 ModelMatrix = modelTransform.getModelMatrix();

        // Send our transformation to the currently bound shader,
        matrix4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        mesh.draw();

        //End Render
        testWindow.updateBuffer();

        }

	testWindow.closeWindow();

	return 0;
}
