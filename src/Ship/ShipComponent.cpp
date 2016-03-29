#include "Ship/ShipComponent.hpp"

ShipComponent::ShipComponent()
{
	m_shipCells = Ship_Map();
}

void ShipComponent::addCell(vector3S pos, ShipCell* cell)
{
	m_shipCells[pos] = cell;
}

void ShipComponent::removeCell(vector3S pos)
{
	if (hasCell(pos))
	{
		delete m_shipCells[pos];
		m_shipCells.erase(pos);
	}
}

ShipCell* ShipComponent::getCell(vector3S pos)
{
	if (hasCell(pos))
	{
		return m_shipCells[pos];
	}

	return nullptr;
}

bool ShipComponent::hasCell(vector3S pos)
{
	return m_shipCells.find(pos) != m_shipCells.end();
}