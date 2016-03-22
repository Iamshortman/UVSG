#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include "Components.hpp"

#include "Camera.hpp"
#include "Window.hpp"
#include "glmInclude.hpp"
#include "ShaderProgram.hpp"
#include "TexturePool.hpp"
#include "EntityxInclude.hpp"
#include "Renderable.hpp"

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
	PointLight(vector3D position, float range, vector3F color, float intensity)
		:BaseLight(color, intensity)
	{
		m_position = position;
		m_range = range;
	};
	vector3D m_position;
	float m_range;

};

#include "World/World.hpp"

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();
	void update(double deltaTime, World* world);

	Camera camera;
	Window* window = nullptr;
	TexturePool texturePool;

	const double farViewScaleValue = 10000.0;

	Model* firstPersonModel = nullptr;
	Model* skybox = nullptr;

protected:

private:
	vector3F ambientLight;
	DirectionalLight* light;
	ShaderProgram* DirectionalShader;
	ShaderProgram* pointShader;
	PointLight* pointLight;
};

#endif //RENDERINGMANAGER_HPP
