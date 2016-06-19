#ifndef SHIPCELLDATA_HPP
#define SHIPCELLDATA_HPP

#include "Rendering/Model/MaterialMesh.hpp"
#include "Directions.hpp"

#include "Ship/ShipCell.hpp"

class ShipCellData
{
public:
	int m_direction = FORWARD;
	vector3B m_position; 

	ShipCellData();
	ShipCellData(ShipCell* cellType, vector3B position, int direction = 0);
	ShipCellData(const ShipCellData &data);
	~ShipCellData();

	double getCellMass();
	bool isCellAtPoint(vector3B point);
	Mesh* getMesh();
	Mesh* getInteriorMesh();
	vector<Node> getNodePoints();
	vector<CockpitSeat> getSeats();
	AABB getAABB();
	void addCollisionShape(vector3D pos, btCompoundShape* shape);
	
	bool isNull();

private:
	ShipCell* m_cellType = nullptr;
};

#endif //SHIPCELLDATA_HPP