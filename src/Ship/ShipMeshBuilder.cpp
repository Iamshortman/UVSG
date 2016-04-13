#include "Ship/ShipMeshBuilder.hpp"
#include "Ship/Directions.hpp"

Mesh* genOutsideMesh(ShipComponent* ship, float cellSize)
{
	float cubeSize = cellSize;

	vector<MaterialVertex> vertices;
	vector<Material> materials;

	for (auto it = ship->m_shipCells.begin(); it != ship->m_shipCells.end(); ++it)
	{
		vector3S pos = it->first;
		ShipCell* cell = it->second;
		cell->addCellToMesh(ship, pos,  cubeSize, vertices, materials);
	}

	//If there are no cells
	if (vertices.size() == 0)
	{
		return nullptr;
	}

	Material mat = Material();
	mat.name = "Test";
	mat.diffuse_Color = vector3F(0, 1, 1);
	mat.illum_Value = 2;
	materials.push_back(mat);

	return new MaterialMesh(vertices, materials);
}