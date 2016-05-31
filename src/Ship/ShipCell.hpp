#ifndef SHIPCELL_HPP
#define SHIPCELL_HPP

#include "AABB.hpp"
#include "Directions.hpp"
#include "glmInclude.hpp"
#include "Rendering/Mesh.hpp"

#include <vector>

class Node
{
public:
	Node(vector3S position = vector3S(0), int direction = FORWARD);
	bool operator== (const Node& other);

	vector3S m_position;
	int m_direction;
};

class ShipCell
{
public:
	ShipCell(Mesh* mesh, Mesh* cursorMesh, double mass, std::vector<Node> nodes, AABB aabb);
	ShipCell(string jsonFile);

	~ShipCell();

	double getCellMass();
	Mesh* getMesh();
	Mesh* getCursorMesh();
	std::vector<Node> getNodePoints();
	AABB getAABB();

private:
	double m_mass;
	Mesh* m_mesh = nullptr;
	Mesh* m_cursorMesh = nullptr;

	std::vector<Node> m_nodes;
	AABB m_aabb;
};

#endif //SHIPCELL_HPP