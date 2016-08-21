#ifndef SHIPCOMPONENT_HPP
#define SHIPCOMPONENT_HPP

#include "GLM_Include.hpp"
#include "Ship/ShipCellData.hpp"
#include "Ship/Directions.hpp"
#include "Components/Component.hpp"

#include <unordered_map>

struct Quad
{
	Quad(){};
	Quad(vector3F a, vector3F b, vector3F c, vector3F d)
	{
		m_a = a;
		m_b = b;
		m_c = c;
		m_d = d;
	};
	vector3F m_a;
	vector3F m_b;
	vector3F m_c;
	vector3F m_d;
};

struct insideCubeFace
{
	/*
	[00][10][20]
	[01][11][21]
	[02][12][22]
	*/
	Quad m_Faces[3][3];

	//m_Checks is a cell offest to check if the quad in the corresponding m_faces location should be drawn.
	//The Mesh gen function will deal with this logic.
	vector3B m_Checks[3][3];
	DIRECTIONS m_SubstitutionDirections[4];
	vector2I m_SubstitutionChecks[4][3];

	vector3F m_Normal;
};

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
typedef std::unordered_map<vector3B, vector<CockpitSeat>, ShipMapKeyFuncs> Ship_Seat_Map;

class ShipComponent : public Component
{
public:
	Ship_Map m_shipCells;
	Ship_Seat_Map m_seatMap;

	Mesh* outsideMesh = nullptr;
	Mesh* insideMesh = nullptr;

	double shipOutsideSize;
	double shipInsideSize;

	bool hasInterior = false;

	vector3D m_centerOfMass;
	bool isBeingRidden = false;

	ShipComponent(double cubeSizeOutside, double cubeSizeInside = 0.0);
	virtual void update(double deltaTime);

	void initializeEntity();

	void addCell(ShipCellData cell);
	void removeCell(vector3B pos);
	ShipCellData getCell(vector3B pos);
	vector3B getCellRootPos(ShipCellData cell);
	bool hasCellAtPos(vector3B pos);
	bool hasNode(vector3B pos, int direction);
	bool hasInternalNode(vector3B pos, int direction);
	bool canPlaceCell(ShipCellData& cell);
	bool getRayCollision(const vector3D& rayOrigin, const vector3D& rayEnd, vector3D& out_Pos, DIRECTIONS& out_HitFace); 


	Mesh* genOutsideMesh();
	Mesh* genInsideMesh();

	void EjectOccupancy();



};

#endif //SHIPCOMPONENT_HPP