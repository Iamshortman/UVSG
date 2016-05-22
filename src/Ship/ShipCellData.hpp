#ifndef SHIPCELLDATA_HPP
#define SHIPCELLDATA_HPP

#include "Rendering/MaterialMesh.hpp"
#include "Directions.hpp"

#include "Ship/ShipCell.hpp"

#include <vector>
using std::vector;

class ShipCellData
{
public:
	int m_direction = FORWARD;
	vector3S m_position; 

	ShipCellData();
	ShipCellData(ShipCell* cellType, vector3S position, int direction = 0);

	double getCellMass();
	bool isCellAtPoint(vector3S point);
	Mesh* getMesh();
	vector<Node> getNodePoints();
	AABB getAABB();
	bool isNull();


private:
	ShipCell* m_cellType = nullptr;

};

#endif //SHIPCELLDATA_HPP