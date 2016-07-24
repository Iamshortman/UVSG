#include "Ship/ShipComponent.hpp"

#include "Renderable.hpp"
#include "Util.hpp"

ShipComponent::ShipComponent(double shipSize)
{
	this->shipOutsideSize = shipSize;
	m_shipCells = Ship_Map();
}

void ShipComponent::update(double deltaTime)
{
	Entity* parent = getParent();
	if (parent == nullptr)
	{
		printf("Error: No parent!!!\n");
		return;
	}

	bool beingRidden = false;

	//Loop though all the cells
	for (auto it = this->m_shipCells.begin(); it != this->m_shipCells.end(); ++it)
	{
		vector3B cellPos = it->first;
		ShipCellData cell = it->second;

		if (m_seatMap.find(cellPos) != m_seatMap.end())
		{

			for (int i = 0; i < m_seatMap[cellPos].size(); i++)
			{
				Entity* entity = m_seatMap[cellPos][i].m_occupyingEntity;
				if (entity != nullptr)
				{
					beingRidden = true;

					Transform transform = parent->getTransform();
					vector3D pos = transform.getOrientation() * (m_seatMap[cellPos][i].m_position + (vector3D)cellPos - m_centerOfMass);
					transform.m_position += pos;

					entity->setTransform(transform);
				}
			}
		}
	}

	this->isBeingRidden = beingRidden;
}

void ShipComponent::initializeEntity()
{
	Entity* parent = getParent();
	if (parent == nullptr)
	{
		printf("Error: No parent!!!\n");
		return;
	}

	outsideMesh = genOutsideMesh();

	double totalMass = 0.0;
	vector3D centerOfMass = vector3D(0.0);
	int totalCellCount = 0;

	btCompoundShape* shape = new btCompoundShape();

	for (auto it = this->m_shipCells.begin(); it != this->m_shipCells.end(); ++it)
	{
		vector3B pos = it->first;
		vector3D pos1 = (vector3D)pos;
		ShipCellData cell = it->second;

		totalCellCount++;
		double mass = cell.getCellMass();
		totalMass += mass;
		centerOfMass += pos1 * mass;

		cell.addCollisionShape(pos1, shape);

		//Add Seats
		if (cell.getSeats().size() > 0)
		{
			m_seatMap[cell.m_position] = cell.getSeats();
		}
	}

	centerOfMass /= totalMass;

	m_centerOfMass = centerOfMass;

	for (int i = 0; i < shape->getNumChildShapes(); i++)
	{
		btTransform transform = shape->getChildTransform(i);
		transform.setOrigin(transform.getOrigin() - toBtVec3(centerOfMass));
		shape->updateChildTransform(i, transform, false);
	}

	parent->setTransform(parent->getTransform().getPosition() + centerOfMass);
	parent->addRigidBody(new RigidBody(shape, totalMass));
}

void ShipComponent::addCell(ShipCellData cell)
{
	m_shipCells[cell.m_position] = cell;
}

void ShipComponent::removeCell(vector3B pos)
{
	if (m_shipCells.find(pos) != m_shipCells.end())
	{
		m_shipCells.erase(pos);
		return;
	}
}

ShipCellData ShipComponent::getCell(vector3B pos)
{
	if (m_shipCells.find(pos) != m_shipCells.end())
	{
		return m_shipCells[pos];
	}

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		//If the cells Intercet
		if (it->second.getAABB().pointIntersect(pos))
		{
			return it->second;
		}
	}

	return ShipCellData();
}

bool ShipComponent::hasCellAtPos(vector3B pos)
{
	if (m_shipCells.find(pos) != m_shipCells.end())
	{
		return true;
	}

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		//If the cells Intercet
		if (it->second.getAABB().pointIntersect(pos))
		{
			return true;
		}
	}

	return false;
}

bool ShipComponent::hasNode(vector3B pos, int direction)
{
	Node testNode = Node(pos, direction);

	ShipCellData data = getCell(pos);

	if (!data.isNull())
	{
		vector<Node> nodes = data.getNodePoints();

		for (Node node : nodes)
		{
			if (node == testNode)
			{
				return true;
			}
		}
	}

	return false;
}

bool ShipComponent::canPlaceCell(ShipCellData& cell)
{
	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		//If the cells Intercet
		if (cell.getAABB().aabbIntersect(it->second.getAABB()))
		{
			return false;
		}
	}

	return true;
}

bool ShipComponent::getRayCollision(const vector3D& rayOrigin, const vector3D& rayEnd, vector3D& out_Pos, DIRECTIONS& out_HitFace)
{
	vector3D tempPos = vector3D();
	DIRECTIONS tempDir = (DIRECTIONS)-1;
	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		//If the cells Intercet
		if (it->second.getAABB().rayIntersect(rayOrigin, rayEnd, tempPos, tempDir))
		{
			if (tempDir == -1)
			{
				return false;
			}

			out_Pos = tempPos;
			out_HitFace = tempDir;

			return true;
		}
	}

	return false;
}

Mesh* ShipComponent::genOutsideMesh()
{
	float cubeSize = (float)shipOutsideSize;
	vector3F vertsCube[] =
	{
		vector3F(-0.5f, -0.5f, -0.5f) * cubeSize,
		vector3F(-0.5f, -0.5f, 0.5f) * cubeSize,
		vector3F(-0.5f, 0.5f, -0.5f) * cubeSize,
		vector3F(-0.5f, 0.5f, 0.5f) * cubeSize,
		vector3F(0.5f, -0.5f, -0.5f) * cubeSize,
		vector3F(0.5f, -0.5f, 0.5f) * cubeSize,
		vector3F(0.5f, 0.5f, -0.5f) * cubeSize,
		vector3F(0.5f, 0.5f, 0.5f) * cubeSize,
	};
	vector3F normals[] =
	{
		vector3F(0, 1, 0),
		vector3F(0, -1, 0),
		vector3F(0, 0, 1),
		vector3F(0, 0, -1),
		vector3F(1, 0, 0),
		vector3F(-1, 0, 0),
	};

	vector3B cellOffset[] =
	{
		vector3B(0, 1, 0),
		vector3B(0, -1, 0),
		vector3B(0, 0, 1),
		vector3B(0, 0, -1),
		vector3B(1, 0, 0),
		vector3B(-1, 0, 0),
	};

	int indices[6][4] = 
	{
		{3, 7, 6, 2},
		{5, 1, 0, 4},
		{7, 3, 1, 5},
		{2, 6, 4, 0},
		{6, 7, 5, 4},
		{3, 2, 0, 1},
	};


	vector<MaterialVertex> vertices;
	vector<Material> materials;

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		vector<Node> nodes = it->second.getNodePoints();

		for (Node node : nodes)
		{
			vector3F offset = (vector3F)node.m_position * cubeSize;

			int dir = node.m_direction;

			if (!hasNode(node.m_position + cellOffset[dir], flipDirection(dir))) // has node node.m_position + cellOffset[dir] and reverse direction;
			{
				vertices.push_back({ vertsCube[indices[dir][0]] + offset, normals[dir], 0 });
				vertices.push_back({ vertsCube[indices[dir][1]] + offset, normals[dir], 0 });
				vertices.push_back({ vertsCube[indices[dir][2]] + offset, normals[dir], 0 });

				vertices.push_back({ vertsCube[indices[dir][2]] + offset, normals[dir], 0 });
				vertices.push_back({ vertsCube[indices[dir][3]] + offset, normals[dir], 0 });
				vertices.push_back({ vertsCube[indices[dir][0]] + offset, normals[dir], 0 });
			}
		}
	}

	//If there are no cells
	if (vertices.size() == 0)
	{
		return nullptr;
	}

	Material mat = Material();
	mat.name = "Test";
	mat.diffuse_Color = vector3F(0, 1, 1);
	mat.illum_Value = 2;
	materials.push_back(mat);

	return new MaterialMesh(vertices, materials);
}

Mesh* ShipComponent::genInsideMesh()
{

}

void ShipComponent::EjectOccupancy()
{
	//Loop though all the cells
	for (auto it = this->m_shipCells.begin(); it != this->m_shipCells.end(); ++it)
	{
		vector3B cellPos = it->first;
		ShipCellData cell = it->second;

		//If the cell has a cockpit, update the position of the occupying entities
		if (m_seatMap.find(cellPos) != m_seatMap.end())
		{
			for (int i = 0; i < m_seatMap[cellPos].size(); i++)
			{
				Entity* entity = m_seatMap[cellPos][i].m_occupyingEntity;
				if (entity != nullptr)
				{
					entity->ridingEntity = nullptr;
					m_seatMap[cellPos][i].m_occupyingEntity = nullptr;
				}
			}
		}
	}
}