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
	ShaderProgram(string VertexShaderLoc, string FragmentShaderLoc, AttributeLocation list[], int count);
	~ShaderProgram();
	void setActiveProgram();
	void deactivateProgram();
	void deleteProgram();

	void setUniform(string name, matrix4& matrix);
	void setUniform(string name, matrix3& matrix);
	void setUniform(string name, vector3& vec);
	void setUniform(string name, vector2& vec);
	void setUniform(string name, quaternion& quat);

private:
	GLuint vertexID, fragmentID;
	GLuint buildShader(string location, GLuint type);

};

#endif
