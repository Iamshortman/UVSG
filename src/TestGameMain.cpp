#include "Camera.hpp"
#include "Window.hpp"
#include "ShaderProgram.hpp"
#include "openGL.hpp"
#include "glmInclude.hpp"
#include "Mesh.hpp"
#include "PhysicsWorld.hpp"
#include "World.hpp"
#include "GameObject.hpp"
#include "VoxelObject.hpp"

#include "MeshComponent.hpp"
#include "RigidBodyComponent.hpp"

#include <SDL2/SDL.h>
#undef main //Not sure if this is needed

#include <stdlib.h>
#include <math.h>


#include "InputButton.hpp"

using namespace std;

void printVector3(const btVector3 &vec)
{
        cout << "{"
        << vec.getX() << ", " << vec.getY() << ", "<< vec.getZ()
        << "}" << endl;
}

void push3(vector<unsigned int>* vector, unsigned int a, unsigned int b, unsigned int c)
{
    vector->push_back(a - 1);
    vector->push_back(b - 1);
    vector->push_back(c - 1);
}

int roundBtScalar(btScalar num)
{
	return (int)(num + 0.5f);
}

int main()
{
	for (int i = 0; i < 11; i++)
	{
		btScalar num = 0.1 * i;
		cout << num << "= " << roundBtScalar(num) << endl;
	}

	string Title = "UVSG";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	Window testWindow(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
    testWindow.setBufferClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    InputButton input = InputButton();

    Camera camera = Camera();
    camera.moveCameraPos(btVector3(0.0, 10.0F, 25.0F));
    //camera.rotateCamera(camera.getRight(), -17.0F / 57.2957795F);


    World world = World();

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

    Mesh boxMesh = Mesh();
    Mesh groundMesh = Mesh();

    vector<vector3> vertices = vector<vector3>();
    vertices.push_back( vector3( 1.0f, -1.0f, -1.0f) );
    vertices.push_back( vector3( 1.0f, -1.0f,  1.0f) );
    vertices.push_back( vector3(-1.0f, -1.0f,  1.0f) );
    vertices.push_back( vector3(-1.0f, -1.0f, -1.0f) );
    vertices.push_back( vector3( 1.0f,  1.0f, -1.0f) );
    vertices.push_back( vector3( 1.0f,  1.0f,  1.0f) );
    vertices.push_back( vector3(-1.0f,  1.0f,  1.0f) );
    vertices.push_back( vector3(-1.0f,  1.0f, -1.0f) );


    vector<vector3> colors = vector<vector3>();
    colors.push_back( vector3(1.0F, 1.0F, 0.0F) );
    colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
    colors.push_back( vector3(1.0F, 0.0F, 1.0F) );
    colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
    colors.push_back( vector3(1.0F, 1.0F, 0.0F) );
    colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
    colors.push_back( vector3(1.0F, 0.0F, 1.0F) );
    colors.push_back( vector3(0.0F, 1.0F, 1.0F) );

    vector<vector3> normals = vector<vector3>();

    vector<unsigned int> indices = vector<unsigned int>();
    push3(&indices, 5, 8, 6);//TOP
    push3(&indices, 8, 7, 6);
    normals.push_back(vector3(0.0F, 1.0F, 0.0F));
    normals.push_back(vector3(0.0F, 1.0F, 0.0F));
    normals.push_back(vector3(0.0F, 1.0F, 0.0F));

    push3(&indices, 1, 2, 4);//Bottom
    push3(&indices, 2, 3, 4);
    normals.push_back(vector3(0.0F, -1.0F, 0.0F));
    normals.push_back(vector3(0.0F, -1.0F, 0.0F));
    normals.push_back(vector3(0.0F, -1.0F, 0.0F));

    push3(&indices, 2, 6, 3);//Back
    push3(&indices, 6, 7, 3);
    normals.push_back(vector3(0.0F, 0.0F, -1.0F));
    normals.push_back(vector3(0.0F, 0.0F, -1.0F));
    normals.push_back(vector3(0.0F, 0.0F, -1.0F));

    push3(&indices, 1, 4, 8);//Front
    push3(&indices, 5, 1, 8);
    normals.push_back(vector3(0.0F, 0.0F, 1.0F));
    normals.push_back(vector3(0.0F, 0.0F, 1.0F));
    normals.push_back(vector3(0.0F, 0.0F, 1.0F));

    push3(&indices, 1, 5, 2);//Right
    push3(&indices, 5, 6, 2);
    normals.push_back(vector3(1.0F, 0.0F, 0.0F));
    normals.push_back(vector3(1.0F, 0.0F, 0.0F));
    normals.push_back(vector3(1.0F, 0.0F, 0.0F));

    push3(&indices, 3, 7, 4);//Left
    push3(&indices, 7, 8, 4);
    normals.push_back(vector3(-1.0F, 0.0F, 0.0F));
    normals.push_back(vector3(-1.0F, 0.0F, 0.0F));
    normals.push_back(vector3(-1.0F, 0.0F, 0.0F));

    boxMesh.addVertices(vertices, colors, indices);

    vector<vector3> vertices1 = vector<vector3>();
    vector<vector3> colors1 = vector<vector3>();
    vector<vector3> normals1 = vector<vector3>();
    vector<unsigned int> indices1 = vector<unsigned int>();

    vertices1.push_back( vector3( 50.0f, 0.0f,  50.0f) );
    vertices1.push_back( vector3( 50.0f, 0.0f, -50.0f) );
    vertices1.push_back( vector3(-50.0f, 0.0f, -50.0f) );
    vertices1.push_back( vector3(-50.0f, 0.0f,  50.0f) );
    colors1.push_back( vector3(1.0F, 1.0F, 0.0F) );
    colors1.push_back( vector3(0.0F, 1.0F, 1.0F) );
    colors1.push_back( vector3(1.0F, 0.0F, 1.0F) );
    colors1.push_back( vector3(0.0F, 1.0F, 1.0F) );
    push3(&indices1, 1, 2, 3);
    push3(&indices1, 3, 4, 1);
    normals1.push_back( vector3(0.0F, 1.0F, 0.0F) );
    normals1.push_back( vector3(0.0F, 1.0F, 0.0F) );
    normals1.push_back( vector3(0.0F, 1.0F, 0.0F) );
    normals1.push_back( vector3(0.0F, 1.0F, 0.0F) );
    groundMesh.addVertices(vertices1, colors1, normals1, indices1);

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

    unsigned int groundId = world.createGameObject(btVector3(0.0F, -1.0F, 0.0F));
    GameObject* object = world.getGameObject(groundId);
    object->addComponent(new RigidBodyComponent(object,	object->transform, groundShape, 0.0F));
    object->addComponent(new MeshComponent(object, &groundMesh));

    unsigned int voxelId = world.createVoxelObject(btVector3(2.0f, 10.0f, 2.0f));
    VoxelObject* voxel = (VoxelObject*)world.getGameObject(voxelId);


    float time = 0;

    Uint32 lastTime = 0;
    Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 frames = 0;

    bool mouseCaptured = false;

	bool shouldEnd = false;
	while(shouldEnd == false)
	{

        int height, width;
        testWindow.getWindowSize(width, height);

        float screenRes = ((float)width)/((float)height);
        matrix4 ProjectionMatrix = glm::perspective(45.0F, screenRes, 0.1F, 1000.0F);

        //FPS counter stuff
        currentTime = SDL_GetTicks();
        frames++;
        if(currentTime - lastTime > 1000)
        {
            //cout << "FPS: " << frames << endl;
            lastTime = currentTime;
            frames = 0;
        }
        Uint32 delta = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        world.update(delta);

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

		if (input.isMouseButtonDown(SDL_BUTTON_RIGHT) || input.isKeyboardButtonDown(SDL_SCANCODE_N))
		{
			btVector3 rayEnd = camera.getForward() * 1000.0F;
			rayEnd += camera.getPos();
			ClosestRayResultCallback result = world.worldPhysics->rayTest(camera.getPos(), rayEnd);
			if (result.hasHit())
			{
				const btRigidBody* rigidBody = btRigidBody::upcast(result.m_collisionObject);
				GameObject* hitObject = static_cast<GameObject*>(rigidBody->getUserPointer());
				if (hitObject != 0)
				{
					if (hitObject->isVoxel())
					{
						VoxelObject* voxel = static_cast<VoxelObject*>(hitObject);
						btVector3 worldHitPoint = result.m_hitPointWorld;
						
						//Moves the point slightly into the collision shape so there are no rounding errors.
						//worldHitPoint += (camera.getForward() * 0.1f);
						
						matrix4 invModelMatrix;
						voxel->transform.inverse().getOpenGLMatrix(&invModelMatrix[0][0]);
						vector4 localHitPoint = invModelMatrix * vector4(worldHitPoint.getX(), worldHitPoint.getY(), worldHitPoint.getZ(), 1.0f);
						int x, y, z;
						x = roundBtScalar(localHitPoint.x);
						y = roundBtScalar(localHitPoint.y);
						z = roundBtScalar(localHitPoint.z);
						voxel->setBlock(x, y, z, 0);
						cout << "{ " << x << ", " << y << ", " << z << " }\n";
					}
				}
			}
		}

        if(input.isKeyboardButtonDown(SDL_SCANCODE_M))
        {
            unsigned int cubeId = world.createCube(btVector3(20.0f, 40.0f, 20.0f), btVector3(1.0f, 1.0f, 1.0f));
            GameObject* cube = world.getGameObject(cubeId);
            cube->addComponent(new MeshComponent(cube, &boxMesh));
        }

        /*if(input.isKeyboardButtonDown(SDL_SCANCODE_N))
        {
            btVector3 rayEnd = camera.getForward() * 1000.0F;
            rayEnd += camera.getPos();
            GameObject* hitObject = world.rayTrace(camera.getPos(), rayEnd);
            if(hitObject != 0)
            {
                if(hitObject->object_id != groundId)
                {
                    world.deleteGameObject(hitObject->object_id);
                }
            }
        }*/

        float speed = 0.2F;
        if(input.isKeyboardButtonDown(SDL_SCANCODE_W))
        {
            camera.moveCameraPos(camera.getForward() * speed);
        }
        if(input.isKeyboardButtonDown(SDL_SCANCODE_S))
        {
            camera.moveCameraPos(camera.getForward() * -speed);
        }

        if(input.isKeyboardButtonDown(SDL_SCANCODE_D))
        {
            camera.moveCameraPos(camera.getRight() * speed);
        }
        if(input.isKeyboardButtonDown(SDL_SCANCODE_A))
        {
            camera.moveCameraPos(camera.getRight() * -speed);
        }

        if(input.isKeyboardButtonDown(SDL_SCANCODE_SPACE))
        {
            camera.moveCameraPos(btVector3(0.0F, 1.0F, 0.0F) * speed);
        }
        if(input.isKeyboardButtonDown(SDL_SCANCODE_LSHIFT))
        {
            camera.moveCameraPos(btVector3(0.0F, 1.0F, 0.0F) * -speed);
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

        btTransform t;
        matrix4 ModelMatrix = matrix4();
        matrix4 MVP = matrix4();

        for(unsigned int i = 0; i < world.gameObjects.size(); i++)
        {
            GameObject* object = world.getGameObject(i);

            if(object != 0)
            {
                object->transform.getOpenGLMatrix(&ModelMatrix[0][0]);

                // Send our transformation to the currently bound shader,
                MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
                matrix3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MVP)));
                glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
                glUniformMatrix4fv(ModelID, 1, GL_FALSE, &ModelMatrix[0][0]);
                glUniformMatrix3fv(NormalID, 1, GL_FALSE, &NormalMatrix[0][0]);
                object->render();
            }
        }

            //End Render
            testWindow.updateBuffer();

        }

	cout << "# of Game Objects: " << world.gameObjects.size() << endl;
    for(unsigned int i = 0; i < world.gameObjects.size(); i++)
    {
        world.deleteGameObject(i);
    }

	testWindow.closeWindow();

	return 0;
}
