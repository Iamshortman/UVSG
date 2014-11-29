#include "Camera.hpp"
#include "Window.hpp"
#include "Shader/ShaderProgram.hpp"
#include "openGL.hpp"
#include "glmInclude.hpp"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include "btBulletDynamicsCommon.h"

#include "Model/ObjModel.h"
#include "InputButton.hpp"
#include "Transform.hpp"

using namespace std;


std::ostream &operator<< (std::ostream &out, const vector3 &vec)
{
    out << "{"
        << vec.x << ", " << vec.y << ","<< vec.z
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


    ShaderProgram program("VertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

    //START DEMO CODE****************************************************************************************

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(program.ShaderProgramID, "MVP");

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(program.ShaderProgramID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(program.ShaderProgramID, "vertexUV");
    //GLuint vertexNormal_modelspaceID = glGetAttribLocation(program.ShaderProgramID, "vertexNormal_modelspace");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(program.ShaderProgramID, "myTextureSampler");

	// Read our .obj file
	std::vector<vector3> vertices;
	std::vector<vector2> uvs;
	std::vector<vector3> normals; // Won't be used at the moment.
	loadOBJ("cube.obj", vertices, uvs, normals);

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vector3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vector2), &uvs[0], GL_STATIC_DRAW);

    //END DEMO CODE******************************************************************************************

    float time = 0;

    Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 frames = 0;

    Transform modelTransform = Transform();
    modelTransform.setPosition(0.0F, 0.0F, -7.0F);

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

        testWindow.updateBuffer();

        }

	testWindow.closeWindow();

	return 0;
}

//void render()
//{
//       // Projection matrix
//        float screenRes = ((float)width)/((float)height);
//        matrix4 ProjectionMatrix = glm::perspective(45.0F, screenRes, 0.1F, 100.0F);
//
//        matrix4 ViewMatrix = camera.getViewMatrix();
//
//        matrix4 ModelMatrix = modelTransform.getModelMatrix();
//
//        // Send our transformation to the currently bound shader,
//        matrix4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
//
//		// Send our transformation to the currently bound shader,
//		// in the "MVP" uniform
//		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
//
//        // Bind our texture in Texture Unit 0
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, Texture);
//		// Set our "myTextureSampler" sampler to user Texture Unit 0
//		glUniform1i(TextureID, 0);
//
//		// 1rst attribute buffer : vertices
//		glEnableVertexAttribArray(vertexPosition_modelspaceID);
//		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//		glVertexAttribPointer(
//			vertexPosition_modelspaceID,  // The attribute we want to configure
//			3,                            // size
//			GL_FLOAT,                     // type
//			GL_FALSE,                     // normalized?
//			0,                            // stride
//			(void*)0                      // array buffer offset
//		);
//
//		// 2nd attribute buffer : UVs
//		glEnableVertexAttribArray(vertexUVID);
//		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//		glVertexAttribPointer(
//			vertexUVID,                   // The attribute we want to configure
//			2,                            // size : U+V => 2
//			GL_FLOAT,                     // type
//			GL_FALSE,                     // normalized?
//			0,                            // stride
//			(void*)0                      // array buffer offset
//		);
//
//		// Draw the triangles !
//		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
//
//		glDisableVertexAttribArray(vertexPosition_modelspaceID);
//		glDisableVertexAttribArray(vertexUVID);
//
//        //End Render
//
//		testWindow.updateBuffer();
//	}
//
//    // Cleanup VBO
//	glDeleteBuffers(1, &vertexbuffer);
//	glDeleteBuffers(1, &uvbuffer);
