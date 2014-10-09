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

using namespace std;

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

	/*// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(50.0f, screenRatio, 0.1f, 100.0f);
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

    */

	// Load the texture
	GLuint Texture = loadDDS("uvmap.DDS");

    // Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(program.ShaderProgramID, "myTextureSampler");

    short arr[32][32][32];
    int arrSize = 32;

    for(int x = 0; x < arrSize; x++)
    {
        for(int y = 0; y < arrSize; y++)
        {
            for(int z = 0; z < arrSize; z++)
            {
                if(x < 31 && x > 0)
                {
                    if(y < 31 && y > 0)
                    {
                        if(z < 31 && z > 0)
                        {
                            arr[x][y][z] = 1;
                        }
                    }
                }
                arr[x][y][z] = 0;
            }
        }
    }



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


        //Render in here

        time += 0.1F;



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
