#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct AttributeLocation
{
	GLuint index;
	string name;
};

class ShaderProgram
{

public:
    GLuint programID = 0;
	ShaderProgram();
	ShaderProgram(string VertexShaderLoc, string FragmentShaderLoc, vector<AttributeLocation> attributeLocation);
	~ShaderProgram();
	void setActiveProgram();
	void deactivateProgram();

	void setUniform(string name, const float& value);
	void setUniform(string name, const matrix4& matrix);
	void setUniform(string name, const matrix3& matrix);
	void setUniform(string name, const vector3F& vec);
	void setUniform(string name, const vector2F& vec);
	void setUniform(string name, const quaternionF& quat);

private:
	GLuint vertexID, fragmentID;
	GLuint buildShader(string location, GLuint type);

};

#endif
