#ifndef SHIPCELL_HPP
#define SHIPCELL_HPP

#include <btBulletCollisionCommon.h>

#include "AABB.hpp"
#include "Directions.hpp"
#include "GLM_Include.hpp"
#include "Rendering/Model/Mesh.hpp"
#include "Physics/CollisionShape.hpp"

#include <vector>
using std::vector;

class Node
{
public:
	Node(vector3B position = vector3B(0), int direction = FORWARD);
	bool operator== (const Node& other);

	vector3B m_position;
	int m_direction;
};

class CockpitSeat
{
public:
	CockpitSeat(vector3D position, int direction)
	{
		m_position = position;
		m_direction = direction;
	};

	vector3D m_position;
	int m_direction;

	Entity* m_occupyingEntity = nullptr;
};

class ShipCell
{
public:
	ShipCell(Mesh* mesh, Mesh* cursorMesh, double mass, vector<Node> nodes, AABB aabb);
	ShipCell(string jsonFile);

	~ShipCell();

	string getCellId();
	double getCellMass();
	Mesh* getMesh();
	Mesh* getCursorMesh();
	Mesh* getInteriorMesh();
	vector<Node> getNodePoints();
	AABB getAABB();

	CollisionShape* shape;
	vector3D shapeOffset;

	vector<CockpitSeat> m_seats;
	vector<string> m_categories;

private:
	string m_cellId;

	double m_mass;
	Mesh* m_mesh = nullptr;
	Mesh* m_cursorMesh = nullptr;
	Mesh* m_interiorMesh = nullptr;

	vector<Node> m_nodes;
	AABB m_aabb;
};

#endif //SHIPCELL_HPP