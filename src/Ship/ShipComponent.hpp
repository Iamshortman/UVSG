#ifndef SHIPCOMPONENT_HPP
#define SHIPCOMPONENT_HPP

#include "glmInclude.hpp"
#include "Ship/ShipCell.hpp"
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

typedef std::unordered_map<vector3S, ShipCell*, ShipMapKeyFuncs> Ship_Map;

class ShipComponent : public Component
{
public:
	ShipComponent();

	Ship_Map m_shipCells;

	void addCell(vector3S pos, ShipCell* cell);
	void removeCell(vector3S pos);
	ShipCell* getCell(vector3S pos);
	bool hasCell(vector3S pos);

};

#endif //SHIPCOMPONENT_HPP