#include "Camera.hpp"
#include "Screen.hpp"
#include "Shader/ShaderProgram.hpp"
#include "openGL.hpp"
#include "glmInclude.hpp"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "Model/ObjModel.h"
#include "InputButton.hpp"
#include "Transform.hpp"

using namespace std;


GLuint loadDDS(const char * imagepath);

std::ostream &operator<< (std::ostream &out, const vector3 &vec) {
    out << "{"
        << vec.x << ", " << vec.y << ","<< vec.z
        << "}";

    return out;
}

int main()
{

	AE_String Title = "Test Game";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	Screen testScreen(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
    testScreen.setBufferClearColor(0.0, 0.0, 0.0, 1.0);
    InputButton input = InputButton();
    Camera camera = Camera();

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


    // Load the texture
	GLuint Texture = loadDDS("uvmap.DDS");

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

    Transform modelTransform = Transform();
    modelTransform.position = vector3(0.0F, 0.0F, -7.0F);

    modelTransform.update();
    cout << "Forward: " << modelTransform.forward << endl;
    cout << "Up: " << modelTransform.up << endl;
    cout << "Right: " << modelTransform.right << endl;

	bool escKey = false;
	while(escKey == false)
	{
        SDL_Event event;
        while( SDL_PollEvent( &event ) )
        {
            if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                escKey = true;
            }

            input.HandleEvent(event);
            testScreen.HandleEvent(event);
        }

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

        // Projection matrix
        float screenRes = ((float)width)/((float)height);
        matrix4 ProjectionMatrix = glm::perspective(45.0F, screenRes, 0.1F, 100.0F);

        //camera.moveCameraPos(vector3(0.0F, 0.00F, -0.1F));
        //camera.rotateCamera(vector3(0.0F, 1.0F, 0.0), 0.01F);
        matrix4 ViewMatrix = camera.getViewMatrix();

        //modelTransform.pos += vector3(0.0F, 0.0F, -0.01F);


        //float scale = sin(time) + 1;
        //modelTransform.scale = vector3(scale, scale, scale);

        if(input.isKeyboardButtonDown(SDLK_RIGHT))
        modelTransform.rotation *= glm::normalize(glm::angleAxis(0.01F, vector3(0.0F, 0.3F, 0.0F)));

        if(input.isKeyboardButtonDown(SDLK_LEFT))
        modelTransform.rotation *= glm::normalize(glm::angleAxis(-0.01F, vector3(0.0F, 0.3F, 0.0F)));

        //modelTransform.rotation *= glm::normalize(glm::angleAxis(0.01F, vector3(0.3F, 0.0F, 0.0F)));

        modelTransform.update();
        /*cout << "Forward: " << modelTransform.forward << endl;
        cout << "Up: " << modelTransform.up << endl;
        cout << "Right: " << modelTransform.right << endl;*/

        if(input.isKeyboardButtonDown(SDLK_UP))
        {
            modelTransform.position += (modelTransform.forward * .1F);
        }

        if(input.isKeyboardButtonDown(SDLK_DOWN))
        {
            modelTransform.position += -(modelTransform.forward * .1F);
        }

        matrix4 ModelMatrix = modelTransform.getModelMatrix();

        // Send our transformation to the currently bound shader,
        matrix4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(vertexUVID);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);

        //End Render

		testScreen.updateBuffer();
	}

    // Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);

	testScreen.closeScreen();

	return 0;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width  /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	}

	free(buffer);

	return textureID;
}
