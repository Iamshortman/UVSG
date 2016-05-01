#ifndef SHIPCOMPONENT_HPP
#define SHIPCOMPONENT_HPP

#include "glmInclude.hpp"
#include "Ship/ShipCellData.hpp"
#include "Components/Component.hpp"

#include <unordered_map>

struct ShipMapKeyFuncs
{
	size_t operator()(const vector3S& k)const
	{
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const vector3S& a, const vector3S& b)const
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

typedef std::unordered_map<vector3S, ShipCellData, ShipMapKeyFuncs> Ship_Map;

class ShipComponent : public Component
{
public:
	ShipComponent();

	Ship_Map m_shipCells;

	void addCell(ShipCellData cell);
	void removeCell(vector3S pos);
	ShipCellData getCell(vector3S pos);
	vector3S getCellRootPos(ShipCellData cell);
	bool hasCellAtPos(vector3S pos);
	bool hasNode(vector3S pos, int direction);

	bool canPlaceCell(ShipCellData& cell);

	Mesh* genOutsideMesh();
};

#endif //SHIPCOMPONENT_HPP