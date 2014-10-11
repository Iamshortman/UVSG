//GML needs this to not complain about rotations being deprecated
#define GLM_FORCE_RADIANS

#include "Screen.hpp"
#include "Shader/ShaderProgram.hpp"
#include "openGL.hpp"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model/ObjModel.h"
#include "InputButton.hpp"

//Include everthing
#include "Voxel/threed.h"
#include "Voxel/box.h"
#define BOX_VOXEL_SIZE      Vector(0.5f, 0.5f, 0.5f)

using namespace std;

int main()
{

	AE_String Title = "Test Game";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	Screen testScreen(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
    testScreen.setBufferClearColor(0.0, 0.0, 0.0, 1.0);
    InputButton input = InputButton();

    printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );

    ShaderProgram program("VertexShader.vertexshader", "FragmentShader.fragmentshader");

    //START DEMO CODE****************************************************************************************


    	// Get a handle for our "MVP" uniform
	GLuint projectionMatrixID = glGetUniformLocation(program.ShaderProgramID, "projection");
	GLuint viewMatrixID = glGetUniformLocation(program.ShaderProgramID, "view");
    GLuint modelMatrixID = glGetUniformLocation(program.ShaderProgramID, "model");


	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(program.ShaderProgramID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(program.ShaderProgramID, "vertexUV");
    GLuint vertexNormal_modelspaceID = glGetAttribLocation(program.ShaderProgramID, "vertexNormal_modelspace");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(50.0f, 4.0f/3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(program.ShaderProgramID, "myTextureSampler");


    // build a box of size (3,4,6)

    Isosurface *boxIso = new BoxIsosurface(Vector(1.0f, 1.0f, 1.0f));
    Transform boxTrans;
    boxTrans.translate(Vector(0.0f, 0.0f, 0.0f));
    boxIso->setTransform(boxTrans);

    Vector voxelSize = BOX_VOXEL_SIZE;
    IsoMesher_DC mesher(boxIso);
    mesher.setVoxelSize(voxelSize.x(), voxelSize.y(), voxelSize.z());
    Mesh *mesh = mesher.createMesh();
    mesh->computeVertexNormals();

    /*// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ("cube.obj", vertices, uvs, normals);

	// Load it into a VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);*/

    //END DEMO CODE******************************************************************************************

    float time = 0;

	bool escKey = false;
	while(escKey == false)
	{
        if(input.isKeyboardButtonDown(SDLK_ESCAPE))
        {
            SDL_ShowCursor(SDL_ENABLE);
        }

        if(input.isMouseButtonDown(SDL_BUTTON_LEFT))
        {
            SDL_ShowCursor(SDL_DISABLE);
        }

        int height, width;
        testScreen.getScreenSize(width, height);

        //If mouse render is disabled
        if(SDL_ShowCursor(-1) == SDL_DISABLE)
        {
         SDL_WarpMouseInWindow(testScreen.window, width/2, height/2);
        }

        testScreen.clearBuffer();
        program.setActiveProgram();


        time += 0.1F;
        //Render in here

        glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &MVP[0][0]);

        mesh->draw();

        //End Render

		testScreen.updateBuffer();


		SDL_Event event;
		SDL_PollEvent(&event);
		if(event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			escKey = true;
		}

        input.HandleEvent(event);
		testScreen.HandleEvent(event);

	}

    // Cleanup VBO
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);

	testScreen.closeScreen();

	return 0;
}
