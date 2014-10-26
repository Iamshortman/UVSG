//GML needs this to not complain about rotations being deprecated
#define GLM_FORCE_RADIANS

#include "Screen.hpp"
#include "Shader/ShaderProgram.hpp"
#include "openGL.hpp"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <math.h>

#include "Model/ObjModel.h"
#include "InputButton.hpp"

using namespace std;

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "{"
        << vec.x << ", " << vec.y << ","<< vec.z
        << "}";

    return out;
}

struct typeReturn
{
    std::vector<glm::vec3> vertices;
    std::vector<int> faceIndex;
};

struct HermitData
{
    glm::vec3 point;
    glm::vec3 normal;
};

double calcDensity(glm::vec3 pos)
{
    glm::vec3 center(0,0,0);

    glm::vec3 dist = pos - center;

    double sqr_dist = (pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z);
    double sqr_rad = 3 * 3;

    double d = sqr_dist - sqr_rad;
    return d;
}

glm::vec3 calcNormal(glm::vec3 pos)
{
    float xt = pos.x;
    float yt = pos.y;
    float zt = pos.z;

    double d = calcDensity(glm::vec3(xt, yt, zt));
    float nx = calcDensity(glm::vec3(xt + 0.001f, yt, zt)) - d;
    float ny = calcDensity(glm::vec3(xt, yt + 0.001f, zt)) - d;
    float nz = calcDensity(glm::vec3(xt, yt, zt + 0.001f)) - d;

    glm::vec3 normal(nx, ny, nz);
    glm::normalize(normal);

    return normal;
}

glm::vec3 absVec3(glm::vec3 vec)
{
    return glm::vec3(abs(vec.x), abs(vec.y), abs(vec.z));
}

#define TOLERANCE_DENSITY 1e-3
#define TOLERANCE_COORD   1e-5

glm::vec3 calcIntersection_XAxis(glm::vec3 point1, glm::vec3 point2)
{
    float fa, fb;
    float xa, xb;
    if (calcDensity(point1) < 0) // p0 < 0  ,  p1 > 0
    {
        fa = calcDensity(point1);
        fb = calcDensity(point2);
        xa = point1.x;
        xb = point2.x;
    }
    else // p1 < 0  ,  p0 > 0
    {
        fa = calcDensity(point2);
        fb = calcDensity(point1);
        xa = point2.x;
        xb = point1.x;
    }
    float y = point1.y;
    float z = point1.z;
    float xm;
    float density;

    while (1)
    {
        xm = xb - (fb * (xb - xa) / (fb - fa));
       density = calcDensity(glm::vec3(xm, y, z));
       density += 1e-4f;

        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(xa - xb) < TOLERANCE_COORD)
            break;

        if (density < 0)  // pm is negative
        {
            xa = xm;
            fa = density;
        }
        else // pm is positive
        {
            xb = xm;
            fb = density;
        }

        xm = (xa + xb) * 0.5f;
        density = calcDensity(glm::vec3(xm, y, z));
        density += 1e-4f;
        if (density < 0) // pm < 0
        {
            xa = xm;
            fa = density;
        }
        else // pm > 0
        {
            xb = xm;
            fb = density;
        }
    }

    return glm::vec3(xm, y, z);
}

glm::vec3 calcIntersection_YAxis(glm::vec3 point1, glm::vec3 point2)
{
    float fa, fb;
    float ya, yb;
    if (calcDensity(point1) < 0) // p0 < 0  ,  p1 > 0
    {
        fa = calcDensity(point1);
        fb = calcDensity(point2);
        ya = point1.y;
        yb = point2.y;
    }
    else // p1 < 0  ,  p0 > 0
    {
        fa = calcDensity(point2);
        fb = calcDensity(point1);
        ya = point2.y;
        yb = point1.y;
    }
    float x = point1.x;
    float z = point1.z;
    float ym;
    float density;

    while (1)
    {
       ym = yb - (fb * (yb - ya) / (fb - fa));
       density = calcDensity(glm::vec3(x, ym, z));
       density += 1e-4f;

        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(ya - yb) < TOLERANCE_COORD)
            break;

        if (density < 0)  // pm is negative
        {
            ya = ym;
            fa = density;
        }
        else // pm is positive
        {
            yb = ym;
            fb = density;
        }

        ym = (ya + yb) * 0.5f;
        density = calcDensity(glm::vec3(x, ym, z));
        density += 1e-4f;
        if (density < 0) // pm < 0
        {
            ya = ym;
            fa = density;
        }
        else // pm > 0
        {
            yb = ym;
            fb = density;
        }
    }

    return glm::vec3(x, ym, z);
}

glm::vec3 calcIntersection_ZAxis(glm::vec3 point1, glm::vec3 point2)
{
    float fa, fb;
    float za, zb;
    if (calcDensity(point1) < 0) // p0 < 0  ,  p1 > 0
    {
        fa = calcDensity(point1);
        fb = calcDensity(point2);
        za = point1.z;
        zb = point2.z;
    }
    else // p1 < 0  ,  p0 > 0
    {
        fa = calcDensity(point2);
        fb = calcDensity(point1);
        za = point2.z;
        zb = point1.z;
    }
    float x = point1.x;
    float y = point1.y;
    float zm;
    float density;

    while (1)
    {
       zm = zb - (fb * (zb - za) / (fb - fa));
       density = calcDensity(glm::vec3(x, y, zm));
       density += 1e-4f;

        if (fabs(density) < TOLERANCE_DENSITY)
            break;
        if (fabs(za - zb) < TOLERANCE_COORD)
            break;

        if (density < 0)  // pm is negative
        {
            za = zm;
            fa = density;
        }
        else // pm is positive
        {
            zb = zm;
            fb = density;
        }

        zm = (za + zb) * 0.5f;
        density = calcDensity(glm::vec3(x, y, zm));
        density += 1e-4f;
        if (density < 0) // pm < 0
        {
            za = zm;
            fa = density;
        }
        else // pm > 0
        {
            zb = zm;
            fb = density;
        }
    }

    return glm::vec3(x, y, zm);
}

HermitData calcHermitData(glm::vec3 vertex1, glm::vec3 vertex2)
{
    HermitData data;

    if(calcDensity(vertex1) < 0.001F)
    {
        data.point = vertex1;
    }
    else if(calcDensity(vertex2) < 0.001F)
    {
        data.point = vertex2;
    }
    else if(calcDensity(vertex1) - calcDensity(vertex2) < 0.001F)
    {
        data.point = vertex1;
    }
    else
    {
        glm::vec3 diff = vertex1 - vertex2;
        absVec3(diff);
        if (diff.x > 0.001F)
        {
            data.point = calcIntersection_XAxis(vertex1, vertex2);
        }
        else if (diff.y > 0.001F)
        {
            data.point = calcIntersection_YAxis(vertex1, vertex2);
        }
        else if (diff.y > 0.001F)
        {
            data.point = calcIntersection_ZAxis(vertex1, vertex2);
        }

    }

    data.normal = calcNormal(data.point);

    return data;
}

//Checks if the array is all zeros or no zeros
bool checkCubeArray(double (&cube_sign)[8])
{
    int zeros = 0;
    //Checks how many zeros are in the array
    for(int i = 0; i < 8; i++)
    {
        if(cube_sign[i] == 0)
        {
            zeros++;
        }
    }

    //If all zeros or none zeros return true
    if(zeros == 0 || zeros == 8)
    {
        return true;
    }

    return false;
}

typeReturn dualContouring()
{
    //cube_edges

    glm::vec3 cube_vertex[] =
    {
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 1, 0),
        glm::vec3(1, 1, 1)
    };

    const int cube_edge_lenght = 12;
    glm::vec2 cube_edge[] =
    {
        glm::vec2( 0, 1 ),
        glm::vec2( 1, 2 ),
        glm::vec2( 2, 3 ),
        glm::vec2( 3, 0 ),
        glm::vec2( 4, 5 ),
        glm::vec2( 5, 6 ),
        glm::vec2( 6, 7 ),
        glm::vec2( 7, 4 ),
        glm::vec2( 0, 4 ),
        glm::vec2( 1, 5 ),
        glm::vec2( 2, 6 ),
        glm::vec2( 3, 7 )
    };

    glm::vec3 posMin(-5, -5, -5);
    glm::vec3 posMax( 5,  5,  5);
    glm::vec3    res( 1,  1,  1);

    glm::vec3 pos();

    for(int x = posMin.x; x <= posMax.x; x += res.x)
    {
        for(int y = posMin.y; y <= posMax.y; y += res.y)
        {
            for(int z = posMin.z; z <= posMax.z; z += res.z)
            {
                glm::vec3 pos(x, y, z);

                //Get the value at each conner.
                double cube_sign[8];
                for(int i = 0; i < 8; i++)
                {
                    cube_sign[i] = calcDensity(pos + cube_vertex[i]);
                }

                //If the array is all zeros or no zeros skip this step
                if(checkCubeArray(cube_sign))
                {
                    continue;
                }

                HermitData hermitData[12];
                int numIntersections = 0;

                //Really have no idea what it does.
                glm::vec3 masspoint(0,0,0);

                for(int i = 0; i < cube_edge_lenght; i++)
                {
                    glm::vec2 e = cube_edge[i];

                    if(cube_sign[(int)(e.x)] != cube_sign[(int)(e.y)])
                    {
                        glm::vec3 point1 = pos + cube_vertex[(int)(e.x)];
                        glm::vec3 point2 = pos + cube_vertex[(int)(e.y)];
                        hermitData[i] = calcHermitData(point1, point2);
                        masspoint += hermitData[i].point;
                        numIntersections++;
                    }
                }

                //No idea why We do this..
                masspoint /= (float) numIntersections;

                // part 2  compute the QEF-minimizing point
                glm::vec3 newPointNormal(0,0,0);

                double matrix[12][3];
                double vectorArr[12];
                int rows = 0;

                for (int i = 0; i < 12; ++i)
                {
                    const glm::vec3 normal = hermitData[i].normal;

                    matrix[rows][0] = normal.x;
                    matrix[rows][1] = normal.y;
                    matrix[rows][2] = normal.z;

                    glm::vec3 point = hermitData[i].point - masspoint;

                    vectorArr[rows] = (double)glm::dot(normal, point);
                }

                glm::vec3 newPointPos(0,0,0);
                //QEF::evaluate(matrix, vector, rows, &newPointV);
                //newPointV += massPoint;

            }
        }
    }


    return typeReturn();
}

int main()
{
    glm::vec3 vec1(1,1,0);
    glm::vec3 vec2(1,0,1);

    cout << glm::dot(vec2, vec1) << endl;

	AE_String Title = "Test Game";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	Screen testScreen(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
    testScreen.setBufferClearColor(0.0, 0.0, 0.0, 1.0);
    InputButton input = InputButton();

    int num_joy = SDL_NumJoysticks();
    printf("%i joysticks were found.\n\n", num_joy);
    for(int i = 0; i < num_joy; i++)
    {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);
        printf("%s\n", SDL_JoystickName(joystick));
    }


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
	//GLuint TextureID  = glGetUniformLocation(program.ShaderProgramID, "myTextureSampler");

    std::vector<glm::vec3> vertices;


    //cout << calcDensity(glm::vec3(0,1,0)) << endl;
    glm::vec3 normal = calcNormal(glm::vec3(1,1,0));

    cout << normal << endl;

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
        //program.setActiveProgram();


        time += 0.1F;
        //Render in here

        glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &MVP[0][0]);

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
