#include "Ship/ShipComponent.hpp"

ShipComponent::ShipComponent()
{
	m_shipCells = Ship_Map();
}

void ShipComponent::addCell(vector3S pos, ShipCellData cell)
{

}

void ShipComponent::removeCell(vector3S pos)
{

}

ShipCellData ShipComponent::getCell(vector3S pos)
{
	return ShipCellData();
}

vector3S ShipComponent::getCellRootPos(ShipCellData cell)
{
	return vector3S();
}

bool ShipComponent::hasCellAtPos(vector3S pos)
{
	return false;
}

bool ShipComponent::canPlaceCell(vector3S pos, ShipCellData& cell)
{
	return true;
}