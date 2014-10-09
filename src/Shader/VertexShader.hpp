#ifndef VERTEXSHADER_HPP
#define VERTEXSHADER_HPP

#include "Shader.hpp"

using namespace std;

class VertexShader: public Shader
{

public:
    VertexShader(string fileloc);
    VertexShader();
    virtual ~VertexShader();

private:
    virtual void BuildShader(string fileloc);

};

#endif
