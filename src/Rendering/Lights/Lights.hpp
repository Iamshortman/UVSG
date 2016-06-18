#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include "GLM_Include.hpp"

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

class DirectionalLight : public BaseLight
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

#endif //LIGHTS_HPP