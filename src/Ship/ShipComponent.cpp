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
		ShipCell* cell = getCell(pos);
		vector3S rootPos = getCellRootPos(cell);
		delete cell;
		m_shipCells.erase(rootPos);
	}
}

ShipCell* ShipComponent::getCell(vector3S pos)
{
	if (m_shipCells.find(pos) != m_shipCells.end())
	{
		return m_shipCells[pos];
	}

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		ShipCell* cell = it->second;

		vector<vector3S> cellCovered = cell->getCellsCovered();
		for (vector3S cellPos : cellCovered)
		{
			if (cellPos + it->first == pos)
			{
				return cell;
			}
		}
	}

	return nullptr;
}

vector3S ShipComponent::getCellRootPos(ShipCell* cell)
{
	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		if (cell == it->second)
		{
			return it->first;
		}
	}

	return vector3S();
}

bool ShipComponent::hasCell(vector3S pos)
{
	if (m_shipCells.find(pos) != m_shipCells.end())
	{
		return true;
	}

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		ShipCell* cell = it->second;

		vector<vector3S> cellCovered = cell->getCellsCovered();
		for (vector3S cellPos : cellCovered)
		{
			if (cellPos + it->first == pos)
			{
				return true;
			}
		}
	}

	return false;
}

bool ShipComponent::canPlaceCell(vector3S pos, ShipCell* cell)
{
	vector<vector3S> cellCovered = cell->getCellsCovered();
	for (vector3S cellPos : cellCovered)
	{
		cellPos += pos;
		if (hasCell(cellPos))
		{
			return false;
		}
	}

	return true;
}