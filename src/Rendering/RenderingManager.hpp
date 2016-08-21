#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Components/Velocity.hpp"
#include "Components/Transform.hpp"

#include "Camera.hpp"
#include "Window.hpp"
#include "GLM_Include.hpp"
#include "ShaderProgram.hpp"
#include "TexturePool.hpp"
#include "EntityxInclude.hpp"
#include "Renderable.hpp"
#include "GBuffer.hpp"
#include "World/World.hpp"

#include "Rendering/Lights/Lights.hpp"

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();

	//Render
	void RenderMainWorld(double deltaTime, World* world);
	void RenderSecondaryWorld(double deltaTime, World* world);

	void renderModelLight(Camera* camera, Model* model, Transform transform, float alphaValue = 1.0f);
	void renderModel(Camera* camera, Model* model, Transform transform);

	void renderModelLight_FarView(Camera* camera, Model* model, Transform transform, double farViewScaleValue);

	Camera camera;
	Window* window = nullptr;
	TexturePool texturePool;

	Model* skybox = nullptr;

	//TODO Remove in favor of the world based ambient light.
	vector3F ambientLight = vector3F(0.9f);

protected:

private:
	DirectionalLight* directionalLight;
	ShaderProgram* DirectionalShader;

	PointLight* pointLight;
	ShaderProgram* PointShader;
};

#endif //RENDERINGMANAGER_HPP
