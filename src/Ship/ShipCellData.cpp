#include "ShipCellData.hpp"
#include "Util.hpp"

ShipCellData::ShipCellData(ShipCell* cellType, vector3B position, int direction)
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

bool ShipCellData::isCellAtPoint(vector3B point)
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

void ShipCellData::addCollisionShape(vector3D pos, btCompoundShape* shape)
{
	if (m_cellType->shape != nullptr)
	{
		btCollisionShape* childShape = m_cellType->shape->getCollisionShape();
		int bitPackedPos = getPackedPos(m_position);

		childShape->setUserIndex(bitPackedPos);
		shape->addChildShape(btTransform(btQuaternion(0, 1, 0, 1).normalize(), toBtVec3(pos + m_cellType->shapeOffset)), childShape);
	}
}

bool ShipCellData::isNull()
{
	return m_cellType == nullptr;
}