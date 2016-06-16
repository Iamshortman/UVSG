#ifndef Material_HPP
#define Material_HPP

#include "GLM_Include.hpp"

struct Material
{
public:
	std::string name;
	vector3F diffuse_Color;
	//vector3F specular_Color;
	int illum_Value;
	//float shininess_Value;
	float alpha_Value;
};

#endif //Material_HPP