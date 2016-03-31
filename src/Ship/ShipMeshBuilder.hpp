#ifndef SHIPMESHBUILDER_HPP
#define SHIPMESHBUILDER_HPP

#include "Ship/ShipComponent.hpp"
#include "Rendering/MaterialMesh.hpp"

using namespace std;

Mesh* genOutsideMesh(ShipComponent* ship, float cellSize);

#endif //SHIPMESHBUILDER_HPP