#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "openGL.hpp"

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
    GLuint programID;
	ShaderProgram(string VertexShaderLoc, string FragmentShaderLoc, AttributeLocation list[], int count);
	~ShaderProgram();
	void setActiveProgram();
	void deactivateProgram();

private:
	GLuint vertexID, fragmentID;
	GLuint buildShader(string location, GLuint type);

};

#endif
