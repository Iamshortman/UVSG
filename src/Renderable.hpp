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

class Renderable
{
public:
    vector<Model*> models;
};

class FarZoneRenderable : public Renderable
{

};

class NearZoneRenderable : public Renderable
{

};

#endif //RENDERABLE_HPP
