#include "ShipCellData.hpp"

ShipCellData::ShipCellData(ShipCell* cellType, vector3S position, int direction)
{
	m_cellType = cellType;
	m_position = position;
	m_direction = direction;
}

ShipCellData::ShipCellData()
{
}

double ShipCellData::getCellMass()
{
	return m_cellType->getCellMass();
};

bool ShipCellData::isCellAtPoint(vector3S point)
{
	return getAABB().pointIntersect((vector3D)point);
}

Mesh* ShipCellData::getMesh()
{
	return m_cellType->getMesh();
}

vector<Node> ShipCellData::getNodePoints()
{
	vector<Node> nodes;
	for (Node node : m_cellType->getNodePoints())
	{
		nodes.push_back(Node(node.m_position + this->m_position, node.m_direction));
	}
	return nodes;
}

AABB ShipCellData::getAABB()
{
	AABB aabb = m_cellType->getAABB();
	aabb.setPos(m_position);
	return aabb;
}

bool ShipCellData::isNull()
{
	return m_cellType == nullptr;
}