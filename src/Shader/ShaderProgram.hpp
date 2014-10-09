#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

#include "VertexShader.hpp"
#include "FragmentShader.hpp"
#include "../openGL.hpp"

using namespace std;

class ShaderProgram
{

    public:
    ShaderProgram(string vertexLoc, string fragmentLoc);
    ~ShaderProgram();
    void setActiveProgram();
    GLuint ShaderProgramID;

    private:
    VertexShader vertexShader;
    FragmentShader fragmentShader;

};

#endif
