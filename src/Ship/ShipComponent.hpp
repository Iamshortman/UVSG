#ifndef SHIPCOMPONENT_HPP
#define SHIPCOMPONENT_HPP

#include "GLM_Include.hpp"
#include "Ship/ShipCellData.hpp"
#include "Ship/Directions.hpp"
#include "Components/Component.hpp"

#include <unordered_map>

struct ShipMapKeyFuncs
{
	size_t operator()(const vector3B& k)const
	{
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const vector3B& a, const vector3B& b)const
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

typedef std::unordered_map<vector3B, ShipCellData, ShipMapKeyFuncs> Ship_Map;

class ShipComponent : public Component
{
public:
	ShipComponent(double shipSize);

	Ship_Map m_shipCells;

	void initializeEntity();

	void addCell(ShipCellData cell);
	void removeCell(vector3B pos);
	ShipCellData getCell(vector3B pos);
	vector3B getCellRootPos(ShipCellData cell);
	bool hasCellAtPos(vector3B pos);
	bool hasNode(vector3B pos, int direction);
	bool canPlaceCell(ShipCellData& cell);

	bool getRayCollision(const vector3D& rayOrigin, const vector3D& rayEnd, vector3D& out_Pos, DIRECTIONS& out_HitFace);

	Mesh* outsideMesh;
	Mesh* genOutsideMesh();

	double shipOutsideSize;

	vector3D m_centerOfMass;
};

#endif //SHIPCOMPONENT_HPP