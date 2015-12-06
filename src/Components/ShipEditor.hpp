#ifndef SHIPEDITOR_HPP
#define SHIPEDITOR_HPP

#include <unordered_map>

#include "glmInclude.hpp"
#include "Rendering/ColoredMesh.hpp"

struct ShipCell
{

};

struct KeyFuncs
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

class ShipEditor
{
public:
	ShipEditor();
	virtual ~ShipEditor();

	void Update();

	std::unordered_map<vector3S, int, KeyFuncs, KeyFuncs> m_shipCells;
	ColoredMesh* m_OutsideMesh;
	ColoredMesh* m_InsideMesh;

	vector3S m_cursorPos;
};

#endif //SHIPEDITOR_HPP