#include "ShipCell.hpp"

Node::Node(vector3S position, int direction)
{
	m_position = position;
	m_direction = direction;
}

ShipCell::ShipCell(Mesh* mesh, double mass, std::vector<Node> nodes, std::vector<vector3S> points)
{
	m_mesh = mesh;
	m_mass = mass;
	m_nodes = nodes;
	m_points = points;
}

ShipCell::~ShipCell()
{
	delete m_mesh;
}

bool ShipCell::isCellAtPoint(vector3S point)
{
	//If the cell is at point return true;
	if (std::find(m_points.begin(), m_points.end(), point) != m_points.end())
	{
		return true;
	}

	return false;
}

std::vector<Node> ShipCell::getNodePoints()
{
	return m_nodes;
}