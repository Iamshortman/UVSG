#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "Mesh.hpp"

struct MeshComponent
{
	Mesh mesh;
	vector3 offset;
};

#endif // COMPONETS_HPP