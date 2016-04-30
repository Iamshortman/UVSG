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
	vector<Node> nodes = m_cellType->getNodePoints();
	for (Node node : nodes)
	{
		node.m_position += m_position;
	}
	return nodes;
}