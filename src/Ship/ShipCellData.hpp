#ifndef SHIPCELLDATA_HPP
#define SHIPCELLDATA_HPP

#include "Rendering/Model/MaterialMesh.hpp"
#include "Directions.hpp"

#include "Ship/ShipCell.hpp"

#include <vector>
using std::vector;

class ShipCellData
{
public:
	int m_direction = FORWARD;
	vector3B m_position; 

	ShipCellData();
	ShipCellData(ShipCell* cellType, vector3B position, int direction = 0);

	double getCellMass();
	bool isCellAtPoint(vector3B point);
	Mesh* getMesh();
	vector<Node> getNodePoints();
	AABB getAABB();
	void addCollisionShape(vector3D pos, btCompoundShape* shape);
	
	bool isNull();


private:
	ShipCell* m_cellType = nullptr;

};

#endif //SHIPCELLDATA_HPP