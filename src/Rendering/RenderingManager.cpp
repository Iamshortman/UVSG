#include "RenderingManager.hpp"

#include <iostream>
#include "Renderable.hpp"
#include "UVSG.hpp"
#include "ObjLoader.hpp"

RenderingManager::RenderingManager()
{
	string Title = "UVSG";
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 600;

	window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
	window->setBufferClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	camera = Camera();
	camera.moveCameraPos(vector3D(0, 1, -10));

	directionalLight = new DirectionalLight(vector3F(1.0f, -1.0f, 1.0f), vector3F(1.0f, 1.0f, 1.0f), 0.6f);
	DirectionalShader = new ShaderProgram("res/Material.vs", "res/foward-directional.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
	pointLight = new PointLight(vector3D(0.0, 10.0, 0.0), 20.0f, vector3F(1.0), 0.4f, vector3F(0.0f, 0.0f, 0.02f));
	PointShader = new ShaderProgram("res/Material.vs", "res/foward-point.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });

	gBuffer = new GBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	gBuffer_Program = new ShaderProgram("res/Shaders/LightingPass.vs", "res/Shaders/LightingPass.fs", { { 0, "in_Position" }, { 1, "in_TexCoord" } });

	gBuffer_Program->setActiveProgram();
	gBuffer_Program->setUniform("gPosition", 0);
	gBuffer_Program->setUniform("gNormal", 1);
	gBuffer_Program->setUniform("gColor", 2);
}

RenderingManager::~RenderingManager()
{
	delete skybox;

	delete gBuffer;
	delete gBuffer_Program;

	window->closeWindow();
	delete window;
}

void RenderingManager::update(double timeStep, World* world)
{
	int width, height;
	window->getWindowSize(width, height);
	camera.setProjection(45.0f, 0.01f, 1000.0f, width, height);

	//Clear depth buffer so any other object will render in front of far objects.
	//glClear(GL_DEPTH_BUFFER_BIT);

	//world->renderFarView(&camera);

	//Clear depth buffer so any other object will render in front of far objects.
	//glClear(GL_DEPTH_BUFFER_BIT);

	gBuffer->BindGBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	world->render(&camera);

	//Unbind GBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render GBuffer
	gBuffer_Program->setActiveProgram();
	gBuffer->SetActiveTextures();

	RenderQuad();

	gBuffer_Program->deactivateProgram();

	window->updateBuffer();
}

void RenderingManager::RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RenderingManager::renderModel(Camera* camera, Model* model, Transform transform)
{

	matrix4 projectionMatrix = camera->getProjectionMatrix();
	matrix4 viewMatrix = camera->getOriginViewMatrix();

	matrix4 modelMatrix = transform.getModleMatrix(camera->getPos());
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

	matrix4 modelMatrix = transform.getModleMatrix(camera->getPos());
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
	PointShader->setUniform("pointLight.positionWorld", (vector3F)(pointLight->m_position - camera->getPos()));
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

	matrix4 modelMatrix = transform.getModleMatrix(camera->getPos(), farViewScaleValue);
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
	PointShader->setUniform("pointLight.positionWorld", (vector3F)(pointLight->m_position - camera->getPos()));
	PointShader->setUniform("pointLight.atten.constant", pointLight->m_attenuation.x);
	PointShader->setUniform("pointLight.atten.linear", pointLight->m_attenuation.y);
	PointShader->setUniform("pointLight.atten.exponent", pointLight->m_attenuation.z);
	model->mesh->draw(PointShader);
	PointShader->deactivateProgram();*/

	glDepthFunc(GL_LESS);
	glDepthMask(true);
	glDisable(GL_BLEND);
}