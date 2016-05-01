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
	return m_cellType->isCellAtPoint(point - m_position);
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

vector<vector3S> ShipCellData::getCellPoints()
{
	vector<vector3S> points;
	for (vector3S point : m_cellType->getCellPoints())
	{
		points.push_back(point + this->m_position);
	}
	return points;
}

bool ShipCellData::isNull()
{
	return m_cellType == nullptr;
}