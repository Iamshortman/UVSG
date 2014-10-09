#ifndef FRAGMENTSHADER_HPP
#define FRAGMENTSHADER_HPP

#include "Shader.hpp"

using namespace std;

class FragmentShader: public Shader
{

public:
    FragmentShader(string fileloc);
    FragmentShader();
    virtual ~FragmentShader();

private:
    virtual void BuildShader(string fileloc);

};

#endif
