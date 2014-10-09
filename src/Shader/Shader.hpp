#ifndef SHADER_HPP
#define SHADER_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include "../openGL.hpp"



using namespace std;

class Shader
{

public:
    GLuint ShaderID;

private:
    virtual void BuildShader(string fileloc) = 0;

};

#endif
