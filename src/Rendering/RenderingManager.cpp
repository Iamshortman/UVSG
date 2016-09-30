#include "RenderingManager.hpp"

#include <iostream>
#include "Renderable.hpp"
#include "UVSG.hpp"
#include "ObjLoader.hpp"

RenderingManager::RenderingManager()
{
	string Title = "USG";
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 600;

	window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
	window->setBufferClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	camera = Camera();
	camera.moveCameraPos(vector3D(0, 1, -10));

	//Load Skybox
	skybox = new Model();
	skybox->mesh = loadMeshFromFile("res/Skybox.obj");
	skybox->texture = "res/Skybox.png";
	skybox->shader = new ShaderProgram("res/Shaders/Textured.Deferred.vs", "res/Shaders/Textured.Deferred.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });
	texturePool.loadTexture(skybox->texture);

	gbuffer = new GBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, false);
	
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Create and compile our GLSL program from the shaders
	postShader = new ShaderProgram("res/Shaders/Deferred.vs", "res/Shaders/Deferred.fs", { { 0, "in_Position" }, { 1, "in_TexCoord" } });
}

RenderingManager::~RenderingManager()
{
	delete skybox;

	window->closeWindow();
	delete window;
}

void RenderingManager::RenderMainWorld(double timeStep, World* world)
{
	static vector2I lastSize;
	vector2I size;
	window->getWindowSize(size.x, size.y);
	camera.setProjection(45.0f, 0.01f, 1000.0f, size.x, size.y);

	if (size != lastSize)
	{
		delete gbuffer;
		gbuffer = new GBuffer(size.x, size.y, false);
		lastSize = size;
	}

	gbuffer->BindGBuffer();
	window->clearBuffer();

	//Skybox Render
	matrix4 projectionMatrix = camera.getProjectionMatrix();
	matrix4 viewMatrix = camera.getOriginViewMatrix();
	skybox->shader->setActiveProgram();
	texturePool.bindTexture(skybox->texture);
	skybox->shader->setUniform("MVP", projectionMatrix * viewMatrix);
	skybox->shader->setUniform("ambientLight", vector3F(1.0f));
	skybox->mesh->draw(skybox->shader);
	skybox->shader->deactivateProgram();

	//Clear depth buffer so any other object in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	world->renderFarView(&camera);

	//Clear depth buffer so any other object will render in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	world->render(&camera);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	window->set2dRendering();
	gbuffer->SetActiveTextures();

	postShader->setActiveProgram();
	postShader->setUniform("gColor", 0);
	postShader->setUniform("gPosition", 1);
	postShader->setUniform("gNormal", 2);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0, // attribute
		3,                              // size
		GL_FLOAT,                       // type
		GL_FALSE,                       // normalized?
		0,                              // stride
		(void*)0                        // array buffer offset
		);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);

	postShader->deactivateProgram();
}

void RenderingManager::RenderSecondaryWorld(double timeStep, World* world)
{
	int width, height;
	window->getWindowSize(width, height);
	camera.setProjection(45.0f, 0.01f, 1000.0f, width, height);

	world->render(&camera);
}

void RenderingManager::renderModel(Camera* camera, Model* model, Transform transform)
{

	matrix4 projectionMatrix = camera->getProjectionMatrix();
	matrix4 viewMatrix = camera->getOriginViewMatrix();

	matrix4 modelMatrix = transform.getModleMatrix(camera->getPosition());
	matrix3 normalMatrix = transform.getNormalMatrix();

	modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
	normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();

	model->shader->setActiveProgram();
	if (model->texture != "")
	{
		this->texturePool.bindTexture(model->texture);
	}

	model->shader->setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
	model->shader->setUniform("normalMatrix", normalMatrix);
	model->shader->setUniform("modelMatrix", modelMatrix);
	model->shader->setUniform("ambientLight", this->ambientLight);

	model->mesh->draw(model->shader);

	model->shader->deactivateProgram();
}

void RenderingManager::renderModelLight(Camera* camera, Model* model, Transform transform, float alphaValue)
{
	matrix4 projectionMatrix = camera->getProjectionMatrix();
	matrix4 viewMatrix = camera->getOriginViewMatrix();

	matrix4 modelMatrix = transform.getModleMatrix(camera->getPosition());
	matrix3 normalMatrix = transform.getNormalMatrix();

	modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
	normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();
	matrix4 MVP = projectionMatrix * viewMatrix * modelMatrix;

	model->shader->setActiveProgram();
	if (model->texture != "")
	{
		this->texturePool.bindTexture(model->texture);
	}

	model->shader->setUniform("MVP", MVP);
	model->shader->setUniform("normalMatrix", normalMatrix);
	model->shader->setUniform("modelMatrix", modelMatrix);
	model->shader->setUniform("ambientLight", this->ambientLight);

	model->shader->setUniform("alphaValue", alphaValue);

	model->mesh->draw(model->shader);

	model->shader->deactivateProgram();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);

	/*DirectionalShader->setActiveProgram();
	DirectionalShader->setUniform("MVP", MVP);
	DirectionalShader->setUniform("normalMatrix", normalMatrix);
	DirectionalShader->setUniform("modelMatrix", modelMatrix);
	DirectionalShader->setUniform("directionalLight.color", directionalLight->getColor());
	DirectionalShader->setUniform("directionalLight.direction", directionalLight->getDirection());
	DirectionalShader->setUniform("directionalLight.intensity", directionalLight->getIntensity());
	model->mesh->draw(DirectionalShader);
	DirectionalShader->deactivateProgram();

	PointShader->setActiveProgram();
	PointShader->setUniform("MVP", MVP);
	PointShader->setUniform("normalMatrix", normalMatrix);
	PointShader->setUniform("modelMatrix", modelMatrix);
	PointShader->setUniform("pointLight.base.color", pointLight->getColor());
	PointShader->setUniform("pointLight.base.intensity", pointLight->getIntensity());
	PointShader->setUniform("pointLight.range", pointLight->m_range);
	PointShader->setUniform("pointLight.positionWorld", (vector3F)(pointLight->m_position - camera->getPosition()));
	PointShader->setUniform("pointLight.atten.constant", pointLight->m_attenuation.x);
	PointShader->setUniform("pointLight.atten.linear", pointLight->m_attenuation.y);
	PointShader->setUniform("pointLight.atten.exponent", pointLight->m_attenuation.z);
	model->mesh->draw(PointShader);
	PointShader->deactivateProgram();*/

	glDepthFunc(GL_LESS);
	glDepthMask(true);
	glDisable(GL_BLEND);
}

void RenderingManager::renderModelLight_FarView(Camera* camera, Model* model, Transform transform, double farViewScaleValue)
{
	matrix4 projectionMatrix = camera->getProjectionMatrix();
	matrix4 viewMatrix = camera->getOriginViewMatrix();

	matrix4 modelMatrix = transform.getModleMatrix(camera->getPosition(), farViewScaleValue);
	matrix3 normalMatrix = transform.getNormalMatrix();

	modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
	normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();
	matrix4 MVP = projectionMatrix * viewMatrix * modelMatrix;

	model->shader->setActiveProgram();
	if (model->texture != "")
	{
		this->texturePool.bindTexture(model->texture);
	}

	model->shader->setUniform("MVP", MVP);
	model->shader->setUniform("normalMatrix", normalMatrix);
	model->shader->setUniform("modelMatrix", modelMatrix);
	model->shader->setUniform("ambientLight", this->ambientLight);

	model->mesh->draw(model->shader);

	model->shader->deactivateProgram();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);

	DirectionalShader->setActiveProgram();
	DirectionalShader->setUniform("MVP", MVP);
	DirectionalShader->setUniform("normalMatrix", normalMatrix);
	DirectionalShader->setUniform("modelMatrix", modelMatrix);
	DirectionalShader->setUniform("directionalLight.color", directionalLight->getColor());
	DirectionalShader->setUniform("directionalLight.direction", directionalLight->getDirection());
	DirectionalShader->setUniform("directionalLight.intensity", directionalLight->getIntensity());
	model->mesh->draw(DirectionalShader);
	DirectionalShader->deactivateProgram();

	/*PointShader->setActiveProgram();
	PointShader->setUniform("MVP", MVP);
	PointShader->setUniform("normalMatrix", normalMatrix);
	PointShader->setUniform("modelMatrix", modelMatrix);
	PointShader->setUniform("pointLight.base.color", pointLight->getColor());
	PointShader->setUniform("pointLight.base.intensity", pointLight->getIntensity());
	PointShader->setUniform("pointLight.range", pointLight->m_range);
	PointShader->setUniform("pointLight.positionWorld", (vector3F)(pointLight->m_position - camera->getPosition()));
	PointShader->setUniform("pointLight.atten.constant", pointLight->m_attenuation.x);
	PointShader->setUniform("pointLight.atten.linear", pointLight->m_attenuation.y);
	PointShader->setUniform("pointLight.atten.exponent", pointLight->m_attenuation.z);
	model->mesh->draw(PointShader);
	PointShader->deactivateProgram();*/

	glDepthFunc(GL_LESS);
	glDepthMask(true);
	glDisable(GL_BLEND);
}