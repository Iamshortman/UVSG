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

std::ostream &operator<< (std::ostream &out, const quat &vec)
{
    out << "{"
        << vec.x << ", " << vec.y << ", "<< vec.z << ", " << vec.w
        << "}";

    return out;
}

void printVector3(const btVector3 &vec)
{
        cout << "{"
        << vec.getX() << ", " << vec.getY() << ", "<< vec.getZ()
        << "}" << endl;
}

void push3(vector<unsigned int>* vector, unsigned int a, unsigned int b, unsigned int c)
{
    vector->push_back(a);
    vector->push_back(b);
    vector->push_back(c);
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
    camera.moveCameraPos(btVector3(0.0F, 0.0F, 0.0F));

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

    vector<vector3> vertices = vector<vector3>();
    vertices.push_back( vector3( 1.0f, -1.0f, -1.0f) );
    vertices.push_back( vector3( 1.0f, -1.0f,  1.0f) );
    vertices.push_back( vector3(-1.0f, -1.0f,  1.0f) );
    vertices.push_back( vector3(-1.0f, -1.0f, -1.0f) );
    vertices.push_back( vector3( 1.0f,  1.0f, -1.0f) );
    vertices.push_back( vector3( 1.0f,  1.0f,  1.0f) );
    vertices.push_back( vector3(-1.0f,  1.0f,  1.0f) );
    vertices.push_back( vector3(-1.0f,  1.0f,  -1.0f) );


    vector<vector3> colors = vector<vector3>();
    colors.push_back( vector3(1.0F, 1.0F, 0.0F) );
    colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
    colors.push_back( vector3(1.0F, 0.0F, 1.0F) );
    colors.push_back( vector3(0.0F, 1.0F, 1.0F));
    colors.push_back( vector3(1.0F, 1.0F, 0.0F) );
    colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
    colors.push_back( vector3(1.0F, 0.0F, 1.0F) );
    colors.push_back( vector3(0.0F, 1.0F, 1.0F));


    vector<unsigned int> indices = vector<unsigned int>();
    push3(&indices, 1, 2, 4);
    push3(&indices, 5, 8, 6);
    push3(&indices, 1, 5, 2);
    push3(&indices, 2, 6, 3);
    push3(&indices, 3, 7, 4);
    push3(&indices, 5, 1, 8);
    push3(&indices, 2, 3, 4);
    push3(&indices, 8, 7, 6);
    push3(&indices, 2, 3, 4);
    push3(&indices, 5, 6, 2);
    push3(&indices, 6, 7, 3);
    push3(&indices, 7, 8, 4);
    push3(&indices, 1, 4, 8);

    cout << indices.size() << endl;

    mesh.addVertices(vertices, colors, indices);

    float time = 0;

    Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 frames = 0;

    btTransform objTrans;
    objTrans.setIdentity();
    objTrans.getOrigin() += btVector3( 0.0F, 0.0F, -3.0F);

    float angle = 0.0F;

    bool mouseCaptured = false;

	bool shouldEnd = false;
	while(shouldEnd == false)
	{

        int height, width;
        testWindow.getWindowSize(width, height);

        float screenRes = ((float)width)/((float)height);
        matrix4 ProjectionMatrix = glm::perspective(45.0F, screenRes, 0.1F, 10000.0F);

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
            mouseCaptured = true;
        }

        if(input.isKeyboardButtonDown(SDL_SCANCODE_W))
        {
            camera.moveCameraPos(camera.getForward() * 0.1F);
        }
        if(input.isKeyboardButtonDown(SDL_SCANCODE_S))
        {
            camera.moveCameraPos(camera.getForward() * -0.1F);
        }

        if(input.isKeyboardButtonDown(SDL_SCANCODE_D))
        {
            camera.moveCameraPos(camera.getRight() * 0.1F);
        }
        if(input.isKeyboardButtonDown(SDL_SCANCODE_A))
        {
            camera.moveCameraPos(camera.getRight() * -0.1F);
        }

        if(input.isKeyboardButtonDown(SDL_SCANCODE_SPACE))
        {
            camera.moveCameraPos(btVector3(0.0F, 1.0F, 0.0F) * 0.1F);
        }
        if(input.isKeyboardButtonDown(SDL_SCANCODE_LSHIFT))
        {
            camera.moveCameraPos(btVector3(0.0F, 1.0F, 0.0F) * -0.1F);
        }


        if(mouseCaptured)
        {
            int x, y;
            testWindow.getMousePos(x, y);
            testWindow.setMousePos(width/2, height/2);
            int deltaX = x - width/2;
            int deltaY = y - height/2;
            float sensitivity = 0.001F;

            camera.rotateCamera(btVector3(0.0F, 1.0F, 0.0F), sensitivity * -deltaX);
            camera.rotateCamera(camera.getRight(), sensitivity * -deltaY);

        }


        testWindow.clearBuffer();
        program.setActiveProgram();

        time += 0.1F;
        //Render in here

        matrix4 ViewMatrix = camera.getViewMatrix();

        matrix4 ModelMatrix = matrix4();
        objTrans.getOpenGLMatrix(&ModelMatrix[0][0]);

        // Send our transformation to the currently bound shader,
        matrix4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        mesh.draw();

        //btQuaternion temp = btQuaternion();
        //temp.setRotation(btVector3(0.0F, 0.0F, 1.0F), angle);
        //objTrans.setRotation(temp);
        //angle += 0.033333F;

        //End Render
        testWindow.updateBuffer();

        }

	testWindow.closeWindow();

	return 0;
}
