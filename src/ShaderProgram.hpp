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
	void deleteProgram();

	void setUniform(string name, const matrix4& matrix);
	void setUniform(string name, const matrix3& matrix);
	void setUniform(string name, const vector3& vec);
	void setUniform(string name, const vector2& vec);
	void setUniform(string name, const quaternion& quat);

private:
	GLuint vertexID, fragmentID;
	GLuint buildShader(string location, GLuint type);

};

#endif
