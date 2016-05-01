#ifndef SHIPCELL_HPP
#define SHIPCELL_HPP

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
	ShipCell(Mesh* mesh, double mass, std::vector<Node> nodes, std::vector<vector3S> points);
	~ShipCell();

	double getCellMass();
	Mesh* getMesh();
	bool isCellAtPoint(vector3S point);
	std::vector<Node> getNodePoints();
	std::vector<vector3S> getCellPoints();

private:
	double m_mass;
	Mesh* m_mesh;
	std::vector<Node> m_nodes;
	std::vector<vector3S> m_points;
};

#endif //SHIPCELL_HPP