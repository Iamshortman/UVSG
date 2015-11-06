#include "RenderingManager.hpp"

#include <iostream>
#include "IcosphereGenerator.hpp"

RenderingManager::RenderingManager()
{
	string Title = "UVSG";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
	window->setBufferClearColor(0.0F, 0.0F, 0.0F, 1.0F);

	camera = Camera();

	vector<AttributeLocation> attributes = { { 0, "in_Position" }, { 1, "in_Color" }, { 2, "in_Normal" } }; 
	basicShader = ShaderProgram("basicVertex.vs", "basicFragment.fs", attributes);

	vector<AttributeLocation> attributes1 = { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } };
	texturedShader = ShaderProgram("TexturedVertex.vs", "TexturedFragment.fs", attributes1);

	//texturedLightShader = ShaderProgram("TexturedLightVertex.vs", "TexturedLightFragment.fs", attributes1, 3);

	StarShader = ShaderProgram("StarVertex.vs", "StarFragment.fs", { { 0, "in_Position" }, { 2, "in_Normal" } });
	BillboardShader = ShaderProgram("BillboardVertex.vs", "BillboardFragment.fs", { { 0, "in_Position" } });

	texturePool.loadTexture("stone.png");
	texturePool.loadTexture("arrow-up.png");

	bool failed = texturePool.bindTexture("stone.png");

	//chunk = TestChunk();

	m_sun.color = vector3(253, 184, 19) / 255.0f;
	m_sun.transform.setPos(vector3(0.0f, 0.0f, 200.0f));
	m_sun.transform.setScale(vector3(100.0f));

	vector<vector3> vertices = vector<vector3>();
	vector<vector3> colors = vector<vector3>();
	vector<vector3> normals = vector<vector3>();
	vector<unsigned int> indices = vector<unsigned int>();

	generateIcosphereMesh(4, vertices, normals, indices);
	for (int i = 0; i < vertices.size(); i++)
	{
		colors.push_back(m_sun.color);
		normals.push_back(glm::normalize(vertices[i]));
	}

	m_sun.starMesh.addVertices(vertices, colors, normals, indices);
	std::printf("Vertex num: %i", vertices.size());

	vertices.clear();
	vertices.push_back(vector3(1, 1, 0));
	vertices.push_back(vector3(-1, 1, 0));
	vertices.push_back(vector3(-1,- 1, 0));
	vertices.push_back(vector3(1, -1, 0));

	indices.clear();

	//Triangle 1
	indices.push_back(0); indices.push_back(2); indices.push_back(1);

	//Triangle 2
	indices.push_back(0); indices.push_back(3); indices.push_back(2);

	m_sun.billboardMesh.addVertices(vertices, colors, normals, indices);
}

void RenderingManager::update(EntityX &entitySystem, double timeStep)
{
	//chunk.update(timeStep);

	int width, height;
	window->getWindowSize(width, height);

	matrix4 viewMatrix = camera.getViewMatrix();

	camera.setProjection(45.0f, 0.1f, 1000.0f, width, height);
	matrix4 projectionMatrix = camera.getProjectionMatrix();
	matrix4 modelMatrix = matrix4();
	matrix4 MVP = matrix4();
	matrix3 normalMatrix = matrix3();

	int count = 0;

	window->clearBuffer();

	//Far object Rendering Start
	/***************************************************************/
	StarShader.setActiveProgram();

	modelMatrix = m_sun.transform.getModleMatrix();
	normalMatrix = m_sun.transform.getNormalMatrix();
	MVP = projectionMatrix * viewMatrix * modelMatrix;

	StarShader.setUniform("MVP", MVP);
	StarShader.setUniform("normalMatrix", normalMatrix);
	StarShader.setUniform("modelMatrix", modelMatrix);
	StarShader.setUniform("starColor", m_sun.color);

	m_sun.starMesh.draw();
	StarShader.deactivateProgram();

	BillboardShader.setActiveProgram();
	BillboardShader.setUniform("starColor", vector3(1.0f) - m_sun.color);

	//Create ModelViewMatrix
	matrix4 modelViewMatrix = viewMatrix * modelMatrix;

	//Clear rotations
	//Commented out lines are to preserver scale.
	// Column 0:
	modelViewMatrix[0][0] = 1;
	modelViewMatrix[0][1] = 0;
	modelViewMatrix[0][2] = 0;

	// Column 1:
	modelViewMatrix[1][0] = 0;
	modelViewMatrix[1][1] = 1;
	modelViewMatrix[1][2] = 0;

	// Column 2:
	modelViewMatrix[2][0] = 0;
	modelViewMatrix[2][1] = 0;
	modelViewMatrix[2][2] = 1;

	//Create MVP matrix
	BillboardShader.setUniform("MVP", projectionMatrix * modelViewMatrix);
	BillboardShader.setUniform("scale", m_sun.transform.scale * 2.0f);
	m_sun.billboardMesh.draw();

	BillboardShader.deactivateProgram();

	//Clear depth buffer so any other object in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	/*****************************************************************/
	//Far object Rendering End

	ComponentHandle<Transform> componentTransformSearch;

	ComponentHandle<TexturedMesh> componentTexturedMeshSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentTexturedMeshSearch, componentTransformSearch))
	{
		ComponentHandle<TexturedMesh> componentMesh = entity.component<TexturedMesh>();
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();

		modelMatrix = componentTransform->getModleMatrix();
		normalMatrix = componentTransform->getNormalMatrix();

		MVP = projectionMatrix * viewMatrix * modelMatrix;

		texturedShader.setActiveProgram();
		texturedShader.setUniform("MVP", MVP);
		texturedShader.setUniform("normalMatrix", normalMatrix);

		texturePool.bindTexture("stone.png");
		componentMesh->draw();

		texturedShader.deactivateProgram();

		count++;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ComponentHandle<MeshComponent> componentMeshSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentMeshSearch, componentTransformSearch))
	{
		ComponentHandle<MeshComponent> componentMesh = entity.component<MeshComponent>();
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();

		modelMatrix = componentTransform->getModleMatrix();
		normalMatrix = componentTransform->getNormalMatrix();

		MVP = projectionMatrix * viewMatrix * modelMatrix;

		basicShader.setActiveProgram();
		basicShader.setUniform("MVP", MVP);
		basicShader.setUniform("offset", componentMesh->offset);
		basicShader.setUniform("normalMatrix", normalMatrix);

		componentMesh->mesh.draw();

		basicShader.deactivateProgram();

		count++;
	}

	//chunk.Render(&basicShader, &camera);

	window->updateBuffer();
}

RenderingManager::~RenderingManager()
{
	basicShader.deleteProgram();
	window->closeWindow();
	delete window;
}