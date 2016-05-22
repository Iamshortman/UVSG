#include "ShipCell.hpp"

Node::Node(vector3S position, int direction)
{
	m_position = position;
	m_direction = direction;
}

bool Node::operator== (const Node& other)
{
	return (m_position == other.m_position) && (m_direction == other.m_direction);
}

ShipCell::ShipCell(Mesh* mesh, Mesh* cursorMesh, double mass, std::vector<Node> nodes, AABB aabb)
{
	m_mesh = mesh;
	m_cursorMesh = cursorMesh;
	m_mass = mass;
	m_nodes = nodes;
	m_aabb = aabb;
}

ShipCell::~ShipCell()
{
	delete m_mesh;
	delete m_cursorMesh;
}

double ShipCell::getCellMass()
{
	return m_mass;
}

Mesh* ShipCell::getMesh()
{
	return m_mesh;
}

Mesh*ShipCell::getCursorMesh()
{
	return m_cursorMesh;
}

std::vector<Node> ShipCell::getNodePoints()
{
	return m_nodes;
}

AABB ShipCell::getAABB()
{
	return m_aabb;
}