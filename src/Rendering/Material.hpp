#ifndef Material_HPP
#define Material_HPP

#include "GLM_Include.hpp"

class Material
{
public:
	Material(){};
	Material(std::string temp, vector3F color, int illum, float alpha)
	{
		name = temp;
		diffuse_Color = color;
		illum_Value = illum;
		alpha_Value = alpha;
	};

	std::string name;
	vector3F diffuse_Color;
	//vector3F specular_Color;
	int illum_Value;
	//float shininess_Value;
	float alpha_Value;
};

#endif //Material_HPP