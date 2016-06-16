#ifndef SHIPCELL_HPP
#define SHIPCELL_HPP

#include <btBulletCollisionCommon.h>

#include "AABB.hpp"
#include "Directions.hpp"
#include "GLM_Include.hpp"
#include "Rendering/Model/Mesh.hpp"
#include "Physics/CollisionShape.hpp"

#include <vector>

class Node
{
public:
	Node(vector3B position = vector3B(0), int direction = FORWARD);
	bool operator== (const Node& other);

	vector3B m_position;
	int m_direction;
};

class ShipCell
{
public:
	ShipCell(Mesh* mesh, Mesh* cursorMesh, double mass, std::vector<Node> nodes, AABB aabb);
	ShipCell(string jsonFile);

	~ShipCell();

	string getCellId();
	double getCellMass();
	Mesh* getMesh();
	Mesh* getCursorMesh();
	std::vector<Node> getNodePoints();
	AABB getAABB();

	CollisionShape* shape;
	vector3D shapeOffset;

private:
	string m_cellId;

	double m_mass;
	Mesh* m_mesh = nullptr;
	Mesh* m_cursorMesh = nullptr;

	std::vector<Node> m_nodes;
	AABB m_aabb;
};

#endif //SHIPCELL_HPP