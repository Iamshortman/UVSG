#include "RenderingManager.hpp"

#include <iostream>
#include "IcosphereGenerator.hpp"
#include "Character.hpp"

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
	
	//m_sun.color = vector3(1.0f);
	m_sun.color = vector3(220, 20, 60) / 255.0f;
	//m_sun.color = vector3(253, 184, 19) / 255.0f;
	m_sun.transform.setPos(vector3(5000.0f, 0.0f, 50000.0f));
	m_sun.transform.setScale(vector3(2500.0f));

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

	vertices.clear();
	vertices.push_back(vector3(1, 1, 0));
	vertices.push_back(vector3(-1, 1, 0));
	vertices.push_back(vector3(-1,- 1, 0));
	vertices.push_back(vector3(1, -1, 0));

	indices.clear();

	//Triangle 1
	indices.push_back(1); indices.push_back(2); indices.push_back(0);

	//Triangle 2
	indices.push_back(2); indices.push_back(3); indices.push_back(0);

	m_sun.billboardMesh.addVertices(vertices, colors, normals, indices);


	vertices.clear();
	colors.clear();
	normals.clear();
	indices.clear();

	int lineCount = 60;
	float insideRadius = 1.5f;
	float outsideRadius = 2.0f;
	const float twicePi = 2 * 3.14159265358979323846;

	for (int i = 0; i < lineCount; i++)
	{
		float x = cosf( i * twicePi / lineCount);
		float y = sinf( i * twicePi / lineCount);

		vertices.push_back(vector3(x, 0, y) * insideRadius);
		colors.push_back(vector3(75, 0, 130) / 255.0f);
		normals.push_back(vector3(0.0f, 1.0f, 0.0f));
		
		vertices.push_back(vector3(x, 0, y) * outsideRadius);
		colors.push_back(vector3(75, 0, 130) / 255.0f);
		normals.push_back(vector3(0.0f, 1.0f, 0.0f));

	}

	for (int i = 0; i < lineCount * 2; i += 2)
	{

		//Final Quad Connets to its self.
		if (i + 2 == lineCount * 2)
		{
			indices.push_back(i + 0);
			indices.push_back(i + 1);
			indices.push_back(0);

			indices.push_back(i + 1);
			indices.push_back(1);
			indices.push_back(0);
		}
		else
		{
			indices.push_back(i + 0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);

			indices.push_back(i + 1);
			indices.push_back(i + 3);
			indices.push_back(i + 2);
		}
	}

	ringMesh = new Mesh();
	ringMesh->addVertices(vertices, colors, normals, indices);
}

void RenderingManager::update(EntityX &entitySystem, double timeStep)
{
	//chunk.update(timeStep);

	int width, height;
	window->getWindowSize(width, height);

	matrix4 viewMatrix = camera.getViewMatrix();

	camera.setProjection(45.0f, 1.0f, 100000.0f, width, height);
	matrix4 projectionMatrix = camera.getProjectionMatrix();
	matrix4 modelMatrix = matrix4();
	matrix4 MVP = matrix4();
	matrix3 normalMatrix = matrix3();

	int count = 0;

	window->clearBuffer();

	//Far object Rendering Start
	/***************************************************************/
	StarShader.setActiveProgram();

	Transform tempTransform = m_sun.transform;

	modelMatrix = tempTransform.getModleMatrix();
	normalMatrix = tempTransform.getNormalMatrix();

	matrix4 modelViewMatrix = viewMatrix * modelMatrix;

	float scaledown = 0.001f;
	modelViewMatrix = glm::scale(modelViewMatrix, vector3(scaledown));
	modelViewMatrix[3][0] *= scaledown;
	modelViewMatrix[3][1] *= scaledown;
	modelViewMatrix[3][2] *= scaledown;

	StarShader.setUniform("MVP", projectionMatrix * modelViewMatrix);
	StarShader.setUniform("normalMatrix", normalMatrix);
	StarShader.setUniform("modelMatrix", modelMatrix);
	StarShader.setUniform("starColor", m_sun.color);

	m_sun.starMesh.draw();
	StarShader.deactivateProgram();

	//Edit ModelViewMatrix
	//Clear rotations
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

	BillboardShader.setActiveProgram();
	BillboardShader.setUniform("starColor", m_sun.color);
	BillboardShader.setUniform("MVP", projectionMatrix * modelViewMatrix);
	BillboardShader.setUniform("scale", tempTransform.getScale() * 3.0f * scaledown);
	m_sun.billboardMesh.draw();
	BillboardShader.deactivateProgram();


	Transform planet;
	planet.setPos(vector3(5000.0f, 0.0f, 0.0f));
	planet.setScale(vector3(1000.0f));
	planet.setOrientation(quaternion(0.959f, 0.0f, 0.0f, 0.284f));

	StarShader.setActiveProgram();

	modelMatrix = planet.getModleMatrix();
	normalMatrix = planet.getNormalMatrix();

	StarShader.setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
	StarShader.setUniform("normalMatrix", normalMatrix);
	StarShader.setUniform("modelMatrix", modelMatrix);
	StarShader.setUniform("starColor", vector3(148, 0, 211) / 255.0f);

	m_sun.starMesh.draw();
	StarShader.deactivateProgram();

	basicShader.setActiveProgram();

	basicShader.setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
	basicShader.setUniform("normalMatrix", normalMatrix);

	ringMesh->draw();

	basicShader.deactivateProgram();

	//Clear depth buffer so any other object in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	/*****************************************************************/
	//Far object Rendering End
	//Set camera param for near rendering
	camera.setProjection(45.0f, 0.1f, 1000.0f, width, height);
	projectionMatrix = camera.getProjectionMatrix();

	ComponentHandle<TexturedMesh> componentTexturedMeshSearch;
	ComponentHandle<Transform> componentTransformSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentTexturedMeshSearch, componentTransformSearch))
	{
		ComponentHandle<TexturedMesh> componentTexturedMesh = entity.component<TexturedMesh>();
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();

		modelMatrix = componentTransform->getModleMatrix();
		normalMatrix = componentTransform->getNormalMatrix();

		MVP = projectionMatrix * viewMatrix * modelMatrix;

		texturedShader.setActiveProgram();
		texturedShader.setUniform("MVP", MVP);
		texturedShader.setUniform("normalMatrix", normalMatrix);

		texturePool.bindTexture("stone.png");
		componentTexturedMesh->draw();

		texturedShader.deactivateProgram();

		count++;
	}

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

	window->updateBuffer();
}

RenderingManager::~RenderingManager()
{
	basicShader.deleteProgram();
	window->closeWindow();
	delete window;
}