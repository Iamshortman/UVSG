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

class BaseLight
{
public:
	BaseLight(vector3F color, float intensity)
	{
		m_color = color;
		m_intensity = intensity;
	};

	vector3F getColor(){ return m_color; };
	float getIntensity(){ return m_intensity; };

	void setColor(vector3F color){ m_color = color; };
	void setIntensity(float intensity){ m_intensity = intensity; };


private:
	vector3F m_color;
	float m_intensity;
};

class DirectionalLight: public BaseLight
{
public:
	DirectionalLight(vector3F direction, vector3F color, float intensity)
		:BaseLight(color, intensity)
	{
		m_direction = direction;
	};

	vector3F getDirection(){ return m_direction; };
	void getDirection(vector3F direction){ m_direction = direction; };

private:
	vector3F m_direction;

};

class PointLight : public BaseLight
{
public:
	PointLight(vector3D position, float range, vector3F color, float intensity, vector3F attenuation)
		:BaseLight(color, intensity)
	{
		m_position = position;
		m_range = range;
		m_attenuation = attenuation;
	};
	vector3D m_position;
	float m_range;
	
	vector3F m_attenuation;
};

#include "World/World.hpp"

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();

	//Render
	void update(double deltaTime, World* world);

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
