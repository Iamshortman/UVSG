#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "Components.hpp"
#include "Rendering/ShaderProgram.hpp"
#include "Rendering/Mesh.hpp"
#include <vector>

struct Model
{
	Mesh* mesh;
	ShaderProgram* shader;
	Transform localOffset;
	string texture;
};

struct Billboard
{
    //vector3D pos;
    vector3F scale;
    ShaderProgram* shader;
    string texture;

    //Temp
    vector3F color;
    Mesh* mesh;
};

class Renderable
{
public:
    vector<Model*> models;
    vector<Billboard*> billboards;
};

class FarZoneRenderable : public Renderable
{

};

class NearZoneRenderable : public Renderable
{

};

#endif //RENDERABLE_HPP
