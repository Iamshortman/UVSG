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

	//Load Skybox
	skybox = new Model();
	skybox->mesh = loadMeshFromFile("res/Skybox.obj");
	skybox->texture = "res/Skybox.png";
	skybox->shader = new ShaderProgram("res/Textured.vs", "res/Textured.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });
	texturePool.loadTexture(skybox->texture);
}

RenderingManager::~RenderingManager()
{
	delete skybox;

	window->closeWindow();
	delete window;
}

void RenderingManager::update(double timeStep, World* world)
{
	int width, height;
	window->getWindowSize(width, height);
	camera.setProjection(45.0f, 0.01f, 1000.0f, width, height);

	window->clearBuffer();

	//Skybox Render
	matrix4 projectionMatrix = camera.getProjectionMatrix();
	matrix4 viewMatrix = camera.getOriginViewMatrix();
	skybox->shader->setActiveProgram();
	texturePool.bindTexture(skybox->texture);
	skybox->shader->setUniform("MVP", projectionMatrix * viewMatrix);
	skybox->mesh->draw(skybox->shader);
	skybox->shader->deactivateProgram();

	//Clear depth buffer so any other object in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	world->renderFarView(&camera);

	//Clear depth buffer so any other object will render in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	world->render(&camera);

	window->updateBuffer();
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