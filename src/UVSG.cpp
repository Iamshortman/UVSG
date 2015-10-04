#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "RenderingManager.hpp"
#include "Components.hpp"
#include "TimeToLive.hpp"
#include "VoxelSystem.hpp"

#include "PlayerControl.hpp"
#include "ThrusterControl.hpp"

UVSG* UVSG::instance;

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool loadOBJ(const char * path, std::vector<vector3> & out_vertices, std::vector<vector2> & out_uvs, std::vector<vector3> & out_normals)
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vector3> temp_vertices;
	std::vector<vector2> temp_uvs;
	std::vector<vector3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0){
			vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){
			vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0){
			vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		vector3 vertex = temp_vertices[vertexIndex - 1];
		vector2 uv = temp_uvs[uvIndex - 1];
		vector3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	return true;
}

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(0);

	entitySystem.systems.add<TimeToLiveSystem>();
	//entitySystem.systems.add<VoxelSystem>();
	entitySystem.systems.add<PlayerControlSystem>();
	//entitySystem.systems.add<ThrusterControlSystem>();
	entitySystem.systems.configure();

	Entity camEntity = entitySystem.entities.create();
	camEntity.assign<Transform>();
	camEntity.assign<CameraLock>();
	camEntity.assign<PlayerControlComponent>(6.0f, 3.0f);
	camEntity.component<Transform>()->position = vector3(20.0f);

	//Sets the camera to look at the center of the world
	camEntity.component<Transform>()->orientation = glm::angleAxis(toRad(45.0f), vector3(1, 0, 0)) * camEntity.component<Transform>()->orientation;
	camEntity.component<Transform>()->orientation = glm::angleAxis(toRad(-135.0f), vector3(0, 1, 0)) * camEntity.component<Transform>()->orientation;


	Entity groundEntity = entitySystem.entities.create();
	groundEntity.assign<Transform>();
	groundEntity.component<Transform>()->position = vector3(0.0f, 0.0f, 0.0f);

	vector<Vertex> verticesStruct = vector<Vertex>();
	Vertex vertex1 = { vector3(-50.0f, 0.0f, 50.0f), vector3(0.0F, 1.0F, 0.0F) , vector2(0.0f, 1.0f)};
	Vertex vertex2 = { vector3(50.0f, 0.0f, 50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(1.0f, 1.0f) };
	Vertex vertex3 = { vector3(-50.0f, 0.0f, -50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(0.0f, 0.0f) };
	Vertex vertex4 = { vector3(50.0f, 0.0f, -50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(1.0f, 0.0f) };
	vector<unsigned int> indicesStruct = {0, 1, 2, 2, 1, 3};
	verticesStruct.push_back(vertex1); verticesStruct.push_back(vertex2); verticesStruct.push_back(vertex3); verticesStruct.push_back(vertex4);
	groundEntity.assign<TexturedMesh>(verticesStruct, indicesStruct);
}


void UVSG::update(double timeStep)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
		}
		renderingManager->window->HandleEvent(event);
	}

	//#1 step Physics
	//physicsWorld->update(entitySystem, timeStep);
	//physxWorld->update(entitySystem, timeStep);

	ComponentHandle<CameraLock> componentCameraSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentCameraSearch))
	{
		ComponentHandle<Transform> transform = entity.component<Transform>();
		renderingManager->camera.setCameraTransform(transform->position, transform->orientation);
	}

	//#2 update Input
	//Update Key Bindings

	//#3 game logic updates
	entitySystem.systems.update_all(timeStep);

	//#4 audio update

	//#5 rendering
	renderingManager->window->set3dRendering();
	renderingManager->update(entitySystem, timeStep);

	//#6 UI Rendering
	renderingManager->window->set2dRendering();

}

Entity UVSG::getEntityFromId(entityxId id)
{
	return entitySystem.entities.get(entitySystem.entities.create_id(id));
}

void UVSG::exitGame()
{
	shouldClose = true;
}

const bool UVSG::getShouldClose()
{
	return shouldClose;
}

UVSG::~UVSG()
{
	int count = 0;
	for (Entity entity : entitySystem.entities.entities_for_debugging())
	{
		entity.destroy();
		count++;
	}
	printf("Num Entities: %i \n", count);

	delete renderingManager;
	//delete physicsWorld;
	//delete physxWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}