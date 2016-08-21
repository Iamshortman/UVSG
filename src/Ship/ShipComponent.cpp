#include "Ship/ShipComponent.hpp"

#include "Renderable.hpp"
#include "Util.hpp"

ShipComponent::ShipComponent(double cubeSizeOutside, double cubeSizeInside)
{
	this->shipOutsideSize = cubeSizeOutside;
	if (cubeSizeInside != 0.0)
	{
		this->shipInsideSize = cubeSizeInside;
		this->hasInterior = true;
	}

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

	if (hasInterior == true)
	{
		insideMesh = genInsideMesh();
	}

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

bool ShipComponent::hasInternalNode(vector3B pos, int direction)
{
	Node testNode = Node(pos, direction);

	ShipCellData data = getCell(pos);

	if (!data.isNull())
	{
		vector<Node> nodes = data.getInternalNodePoints();

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

void PushQuad(std::vector<MaterialVertex>& verticesVector, Quad points, vector3F normal, float material, vector3F offset)
{
	verticesVector.push_back({ points.m_a + offset, normal, material });
	verticesVector.push_back({ points.m_b + offset, normal, material });
	verticesVector.push_back({ points.m_c + offset, normal, material });
	
	verticesVector.push_back({ points.m_c + offset, normal, material });
	verticesVector.push_back({ points.m_d + offset, normal, material });
	verticesVector.push_back({ points.m_a + offset, normal, material });
}

Mesh* ShipComponent::genInsideMesh()
{
	/*if (m_InsideMesh != 0)
	{
		delete m_InsideMesh;
		m_InsideMesh = nullptr;
	}*/

	//from center to outside wall
	float outside = 3.0f / 2.0f;

	//from center to inside wall
	float inside = 2.6f / 2.0f;

	//-------------------------------------------------------------------------
	insideCubeFace ceiling;
	ceiling.m_Normal = vector3F(0, -1, 0);
	ceiling.m_Faces[0][0] = Quad(vector3F(-outside, inside, outside), vector3F(-outside, inside, inside), vector3F(-inside, inside, inside), vector3F(-inside, inside, outside));
	ceiling.m_Faces[1][0] = Quad(vector3F(-inside, inside, outside), vector3F(-inside, inside, inside), vector3F(inside, inside, inside), vector3F(inside, inside, outside));
	ceiling.m_Faces[2][0] = Quad(vector3F(inside, inside, outside), vector3F(inside, inside, inside), vector3F(outside, inside, inside), vector3F(outside, inside, outside));

	ceiling.m_Faces[0][1] = Quad(vector3F(-outside, inside, inside), vector3F(-outside, inside, -inside), vector3F(-inside, inside, -inside), vector3F(-inside, inside, inside));
	ceiling.m_Faces[1][1] = Quad(vector3F(-inside, inside, inside), vector3F(-inside, inside, -inside), vector3F(inside, inside, -inside), vector3F(inside, inside, inside));
	ceiling.m_Faces[2][1] = Quad(vector3F(inside, inside, inside), vector3F(inside, inside, -inside), vector3F(outside, inside, -inside), vector3F(outside, inside, inside));

	ceiling.m_Faces[0][2] = Quad(vector3F(-outside, inside, -inside), vector3F(-outside, inside, -outside), vector3F(-inside, inside, -outside), vector3F(-inside, inside, -inside));
	ceiling.m_Faces[1][2] = Quad(vector3F(-inside, inside, -inside), vector3F(-inside, inside, -outside), vector3F(inside, inside, -outside), vector3F(inside, inside, -inside));
	ceiling.m_Faces[2][2] = Quad(vector3F(inside, inside, -inside), vector3F(inside, inside, -outside), vector3F(outside, inside, -outside), vector3F(outside, inside, -inside));

	ceiling.m_Checks[0][0] = vector3B(-1, 0, 1); //Right Forward
	ceiling.m_Checks[1][0] = vector3B(0, 0, 1); //Forward
	ceiling.m_Checks[2][0] = vector3B(1, 0, 1); //Left Forward

	ceiling.m_Checks[0][1] = vector3B(-1, 0, 0); //Right
	ceiling.m_Checks[1][1] = vector3B(0, 1, 0); //Up
	ceiling.m_Checks[2][1] = vector3B(1, 0, 0); //Left

	ceiling.m_Checks[0][2] = vector3B(-1, 0, -1); //Right Back
	ceiling.m_Checks[1][2] = vector3B(0, 0, -1); //Back
	ceiling.m_Checks[2][2] = vector3B(1, 0, -1); //Left Back

	ceiling.m_SubstitutionDirections[0] = FORWARD; //[1][0]
	ceiling.m_SubstitutionChecks[0][0] = vector2I(0, 0);
	ceiling.m_SubstitutionChecks[0][1] = vector2I(1, 0);
	ceiling.m_SubstitutionChecks[0][2] = vector2I(2, 0);

	ceiling.m_SubstitutionDirections[1] = RIGHT; //[0][1]
	ceiling.m_SubstitutionChecks[1][0] = vector2I(0, 0);
	ceiling.m_SubstitutionChecks[1][1] = vector2I(1, 0);
	ceiling.m_SubstitutionChecks[1][2] = vector2I(2, 0);

	ceiling.m_SubstitutionDirections[2] = BACKWARD; //[1][2]
	ceiling.m_SubstitutionChecks[2][0] = vector2I(0, 0);
	ceiling.m_SubstitutionChecks[2][1] = vector2I(1, 0);
	ceiling.m_SubstitutionChecks[2][2] = vector2I(2, 0);

	ceiling.m_SubstitutionDirections[3] = LEFT; //[2][1]
	ceiling.m_SubstitutionChecks[3][0] = vector2I(0, 0);
	ceiling.m_SubstitutionChecks[3][1] = vector2I(1, 0);
	ceiling.m_SubstitutionChecks[3][2] = vector2I(2, 0);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	insideCubeFace floor;
	floor.m_Normal = vector3F(0, 1, 0);
	floor.m_Faces[0][0] = Quad(vector3F(outside, -inside, outside), vector3F(outside, -inside, inside), vector3F(inside, -inside, inside), vector3F(inside, -inside, outside));
	floor.m_Faces[1][0] = Quad(vector3F(inside, -inside, outside), vector3F(inside, -inside, inside), vector3F(-inside, -inside, inside), vector3F(-inside, -inside, outside));
	floor.m_Faces[2][0] = Quad(vector3F(-inside, -inside, outside), vector3F(-inside, -inside, inside), vector3F(-outside, -inside, inside), vector3F(-outside, -inside, outside));

	floor.m_Faces[0][1] = Quad(vector3F(outside, -inside, inside), vector3F(outside, -inside, -inside), vector3F(inside, -inside, -inside), vector3F(inside, -inside, inside));
	floor.m_Faces[1][1] = Quad(vector3F(inside, -inside, inside), vector3F(inside, -inside, -inside), vector3F(-inside, -inside, -inside), vector3F(-inside, -inside, inside));
	floor.m_Faces[2][1] = Quad(vector3F(-inside, -inside, inside), vector3F(-inside, -inside, -inside), vector3F(-outside, -inside, -inside), vector3F(-outside, -inside, inside));

	floor.m_Faces[0][2] = Quad(vector3F(outside, -inside, -inside), vector3F(outside, -inside, -outside), vector3F(inside, -inside, -outside), vector3F(inside, -inside, -inside));
	floor.m_Faces[1][2] = Quad(vector3F(inside, -inside, -inside), vector3F(inside, -inside, -outside), vector3F(-inside, -inside, -outside), vector3F(-inside, -inside, -inside));
	floor.m_Faces[2][2] = Quad(vector3F(-inside, -inside, -inside), vector3F(-inside, -inside, -outside), vector3F(-outside, -inside, -outside), vector3F(-outside, -inside, -inside));

	floor.m_Checks[0][0] = vector3B(1, 0, 1); //Left Forward
	floor.m_Checks[1][0] = vector3B(0, 0, 1); //Foward
	floor.m_Checks[2][0] = vector3B(-1, 0, 1); //Right Forward

	floor.m_Checks[0][1] = vector3B(1, 0, 0); //Left
	floor.m_Checks[1][1] = vector3B(0, -1, 0); //Down
	floor.m_Checks[2][1] = vector3B(-1, 0, 0); //Right

	floor.m_Checks[0][2] = vector3B(1, 0, -1); //Left Back
	floor.m_Checks[1][2] = vector3B(0, 0, -1); //Back
	floor.m_Checks[2][2] = vector3B(-1, 0, -1); //Right Back

	floor.m_SubstitutionDirections[0] = FORWARD; //[1][0]
	floor.m_SubstitutionChecks[0][0] = vector2I(0, 2);
	floor.m_SubstitutionChecks[0][1] = vector2I(1, 2);
	floor.m_SubstitutionChecks[0][2] = vector2I(2, 2);

	floor.m_SubstitutionDirections[1] = LEFT; //[0][1]
	floor.m_SubstitutionChecks[1][0] = vector2I(0, 2);
	floor.m_SubstitutionChecks[1][1] = vector2I(1, 2);
	floor.m_SubstitutionChecks[1][2] = vector2I(2, 2);

	floor.m_SubstitutionDirections[2] = BACKWARD; //[1][2]
	floor.m_SubstitutionChecks[2][0] = vector2I(0, 2);
	floor.m_SubstitutionChecks[2][1] = vector2I(1, 2);
	floor.m_SubstitutionChecks[2][2] = vector2I(2, 2);

	floor.m_SubstitutionDirections[3] = RIGHT; //[2][1]
	floor.m_SubstitutionChecks[3][0] = vector2I(0, 2);
	floor.m_SubstitutionChecks[3][1] = vector2I(1, 2);
	floor.m_SubstitutionChecks[3][2] = vector2I(2, 2);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	insideCubeFace northWall;
	northWall.m_Normal = vector3F(0, 0, -1);
	northWall.m_Faces[0][0] = Quad(vector3F(outside, outside, inside), vector3F(outside, inside, inside), vector3F(inside, inside, inside), vector3F(inside, outside, inside));
	northWall.m_Faces[1][0] = Quad(vector3F(inside, outside, inside), vector3F(inside, inside, inside), vector3F(-inside, inside, inside), vector3F(-inside, outside, inside));
	northWall.m_Faces[2][0] = Quad(vector3F(-inside, outside, inside), vector3F(-inside, inside, inside), vector3F(-outside, inside, inside), vector3F(-outside, outside, inside));

	northWall.m_Faces[0][1] = Quad(vector3F(outside, inside, inside), vector3F(outside, -inside, inside), vector3F(inside, -inside, inside), vector3F(inside, inside, inside));
	northWall.m_Faces[1][1] = Quad(vector3F(inside, inside, inside), vector3F(inside, -inside, inside), vector3F(-inside, -inside, inside), vector3F(-inside, inside, inside));
	northWall.m_Faces[2][1] = Quad(vector3F(-inside, inside, inside), vector3F(-inside, -inside, inside), vector3F(-outside, -inside, inside), vector3F(-outside, inside, inside));

	northWall.m_Faces[0][2] = Quad(vector3F(outside, -inside, inside), vector3F(outside, -outside, inside), vector3F(inside, -outside, inside), vector3F(inside, -inside, inside));
	northWall.m_Faces[1][2] = Quad(vector3F(inside, -inside, inside), vector3F(inside, -outside, inside), vector3F(-inside, -outside, inside), vector3F(-inside, -inside, inside));
	northWall.m_Faces[2][2] = Quad(vector3F(-inside, -inside, inside), vector3F(-inside, -outside, inside), vector3F(-outside, -outside, inside), vector3F(-outside, -inside, inside));

	northWall.m_Checks[0][0] = vector3B(1, 1, 0); //Up Left
	northWall.m_Checks[1][0] = vector3B(0, 1, 0); //Up
	northWall.m_Checks[2][0] = vector3B(-1, 1, 0); //Up Right

	northWall.m_Checks[0][1] = vector3B(1, 0, 0); //Left
	northWall.m_Checks[1][1] = vector3B(0, 0, 1); //North
	northWall.m_Checks[2][1] = vector3B(-1, 0, 0); //Right

	northWall.m_Checks[0][2] = vector3B(1, -1, 0); //Down Left
	northWall.m_Checks[1][2] = vector3B(0, -1, 0); //Down
	northWall.m_Checks[2][2] = vector3B(-1, -1, 0); //Down Right

	northWall.m_SubstitutionDirections[0] = UP; //[1][0]
	northWall.m_SubstitutionChecks[0][0] = vector2I(0, 0);
	northWall.m_SubstitutionChecks[0][1] = vector2I(1, 0);
	northWall.m_SubstitutionChecks[0][2] = vector2I(2, 0);

	northWall.m_SubstitutionDirections[1] = LEFT; //[0][1]
	northWall.m_SubstitutionChecks[1][0] = vector2I(2, 0);
	northWall.m_SubstitutionChecks[1][1] = vector2I(2, 1);
	northWall.m_SubstitutionChecks[1][2] = vector2I(2, 2);

	northWall.m_SubstitutionDirections[2] = DOWN; //[1][2]
	northWall.m_SubstitutionChecks[2][0] = vector2I(0, 0);
	northWall.m_SubstitutionChecks[2][1] = vector2I(1, 0);
	northWall.m_SubstitutionChecks[2][2] = vector2I(2, 0);

	northWall.m_SubstitutionDirections[3] = RIGHT; //[2][1]
	northWall.m_SubstitutionChecks[3][0] = vector2I(2, 0);
	northWall.m_SubstitutionChecks[3][1] = vector2I(2, 1);
	northWall.m_SubstitutionChecks[3][2] = vector2I(2, 2);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	insideCubeFace southWall;
	southWall.m_Normal = vector3F(0, 0, 1);
	southWall.m_Faces[0][0] = Quad(vector3F(-outside, outside, -inside), vector3F(-outside, inside, -inside), vector3F(-inside, inside, -inside), vector3F(-inside, outside, -inside));
	southWall.m_Faces[1][0] = Quad(vector3F(-inside, outside, -inside), vector3F(-inside, inside, -inside), vector3F(inside, inside, -inside), vector3F(inside, outside, -inside));
	southWall.m_Faces[2][0] = Quad(vector3F(inside, outside, -inside), vector3F(inside, inside, -inside), vector3F(outside, inside, -inside), vector3F(outside, outside, -inside));

	southWall.m_Faces[0][1] = Quad(vector3F(-outside, inside, -inside), vector3F(-outside, -inside, -inside), vector3F(-inside, -inside, -inside), vector3F(-inside, inside, -inside));
	southWall.m_Faces[1][1] = Quad(vector3F(-inside, inside, -inside), vector3F(-inside, -inside, -inside), vector3F(inside, -inside, -inside), vector3F(inside, inside, -inside));
	southWall.m_Faces[2][1] = Quad(vector3F(inside, inside, -inside), vector3F(inside, -inside, -inside), vector3F(outside, -inside, -inside), vector3F(outside, inside, -inside));

	southWall.m_Faces[0][2] = Quad(vector3F(-outside, -inside, -inside), vector3F(-outside, -outside, -inside), vector3F(-inside, -outside, -inside), vector3F(-inside, -inside, -inside));
	southWall.m_Faces[1][2] = Quad(vector3F(-inside, -inside, -inside), vector3F(-inside, -outside, -inside), vector3F(inside, -outside, -inside), vector3F(inside, -inside, -inside));
	southWall.m_Faces[2][2] = Quad(vector3F(inside, -inside, -inside), vector3F(inside, -outside, -inside), vector3F(outside, -outside, -inside), vector3F(outside, -inside, -inside));

	southWall.m_Checks[0][0] = vector3B(-1, 1, 0); //Up Right
	southWall.m_Checks[1][0] = vector3B(0, 1, 0); //Up
	southWall.m_Checks[2][0] = vector3B(1, 1, 0); //Up Left

	southWall.m_Checks[0][1] = vector3B(-1, 0, 0); //Right
	southWall.m_Checks[1][1] = vector3B(0, 0, -1); //South
	southWall.m_Checks[2][1] = vector3B(1, 0, 0); //Left

	southWall.m_Checks[0][2] = vector3B(-1, -1, 0); //Down Right
	southWall.m_Checks[1][2] = vector3B(0, -1, 0); //Down
	southWall.m_Checks[2][2] = vector3B(1, -1, 0); //Down Left

	southWall.m_SubstitutionDirections[0] = UP; //[1][0]
	southWall.m_SubstitutionChecks[0][0] = vector2I(0, 2);
	southWall.m_SubstitutionChecks[0][1] = vector2I(1, 2);
	southWall.m_SubstitutionChecks[0][2] = vector2I(2, 2);

	southWall.m_SubstitutionDirections[1] = RIGHT; //[0][1]
	southWall.m_SubstitutionChecks[1][0] = vector2I(0, 0);
	southWall.m_SubstitutionChecks[1][1] = vector2I(0, 1);
	southWall.m_SubstitutionChecks[1][2] = vector2I(0, 2);

	southWall.m_SubstitutionDirections[2] = DOWN; //[1][2]
	southWall.m_SubstitutionChecks[2][0] = vector2I(0, 2);
	southWall.m_SubstitutionChecks[2][1] = vector2I(1, 2);
	southWall.m_SubstitutionChecks[2][2] = vector2I(2, 2);

	southWall.m_SubstitutionDirections[3] = LEFT; //[2][1]
	southWall.m_SubstitutionChecks[3][0] = vector2I(2, 0);
	southWall.m_SubstitutionChecks[3][1] = vector2I(2, 1);
	southWall.m_SubstitutionChecks[3][2] = vector2I(2, 2);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	insideCubeFace westWall;
	westWall.m_Normal = vector3F(-1, 0, 0);
	westWall.m_Faces[0][0] = Quad(vector3F(inside, outside, -outside), vector3F(inside, inside, -outside), vector3F(inside, inside, -inside), vector3F(inside, outside, -inside));
	westWall.m_Faces[1][0] = Quad(vector3F(inside, outside, -inside), vector3F(inside, inside, -inside), vector3F(inside, inside, inside), vector3F(inside, outside, inside));
	westWall.m_Faces[2][0] = Quad(vector3F(inside, outside, inside), vector3F(inside, inside, inside), vector3F(inside, inside, outside), vector3F(inside, outside, outside));

	westWall.m_Faces[0][1] = Quad(vector3F(inside, inside, -outside), vector3F(inside, -inside, -outside), vector3F(inside, -inside, -inside), vector3F(inside, inside, -inside));
	westWall.m_Faces[1][1] = Quad(vector3F(inside, inside, -inside), vector3F(inside, -inside, -inside), vector3F(inside, -inside, inside), vector3F(inside, inside, inside));
	westWall.m_Faces[2][1] = Quad(vector3F(inside, inside, inside), vector3F(inside, -inside, inside), vector3F(inside, -inside, outside), vector3F(inside, inside, outside));

	westWall.m_Faces[0][2] = Quad(vector3F(inside, -inside, -outside), vector3F(inside, -outside, -outside), vector3F(inside, -outside, -inside), vector3F(inside, -inside, -inside));
	westWall.m_Faces[1][2] = Quad(vector3F(inside, -inside, -inside), vector3F(inside, -outside, -inside), vector3F(inside, -outside, inside), vector3F(inside, -inside, inside));
	westWall.m_Faces[2][2] = Quad(vector3F(inside, -inside, inside), vector3F(inside, -outside, inside), vector3F(inside, -outside, outside), vector3F(inside, -inside, outside));

	westWall.m_Checks[0][0] = vector3B(0, 1, -1); //Up Back
	westWall.m_Checks[1][0] = vector3B(0, 1, 0); //Up
	westWall.m_Checks[2][0] = vector3B(0, 1, 1); //Up Forward

	westWall.m_Checks[0][1] = vector3B(0, 0, -1); //Back
	westWall.m_Checks[1][1] = vector3B(1, 0, 0); //West
	westWall.m_Checks[2][1] = vector3B(0, 0, 1); //Forward

	westWall.m_Checks[0][2] = vector3B(0, -1, -1); //Down Back
	westWall.m_Checks[1][2] = vector3B(0, -1, 0); //Down
	westWall.m_Checks[2][2] = vector3B(0, -1, 1); //Down Forward

	westWall.m_SubstitutionDirections[0] = UP; //[1][0]
	westWall.m_SubstitutionChecks[0][0] = vector2I(2, 0);
	westWall.m_SubstitutionChecks[0][1] = vector2I(2, 1);
	westWall.m_SubstitutionChecks[0][2] = vector2I(2, 2);

	westWall.m_SubstitutionDirections[1] = BACKWARD; //[0][1]
	westWall.m_SubstitutionChecks[1][0] = vector2I(2, 0);
	westWall.m_SubstitutionChecks[1][1] = vector2I(2, 1);
	westWall.m_SubstitutionChecks[1][2] = vector2I(2, 2);

	westWall.m_SubstitutionDirections[2] = DOWN; //[1][2]
	westWall.m_SubstitutionChecks[2][0] = vector2I(0, 0);
	westWall.m_SubstitutionChecks[2][1] = vector2I(0, 1);
	westWall.m_SubstitutionChecks[2][2] = vector2I(0, 2);

	westWall.m_SubstitutionDirections[3] = FORWARD; //[2][1]
	westWall.m_SubstitutionChecks[3][0] = vector2I(0, 0);
	westWall.m_SubstitutionChecks[3][1] = vector2I(0, 1);
	westWall.m_SubstitutionChecks[3][2] = vector2I(0, 2);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	insideCubeFace eastWall;
	eastWall.m_Normal = vector3F(-1, 0, 0);
	eastWall.m_Faces[0][0] = Quad(vector3F(-inside, outside, outside), vector3F(-inside, inside, outside), vector3F(-inside, inside, inside), vector3F(-inside, outside, inside));
	eastWall.m_Faces[1][0] = Quad(vector3F(-inside, outside, inside), vector3F(-inside, inside, inside), vector3F(-inside, inside, -inside), vector3F(-inside, outside, -inside));
	eastWall.m_Faces[2][0] = Quad(vector3F(-inside, outside, -inside), vector3F(-inside, inside, -inside), vector3F(-inside, inside, -outside), vector3F(-inside, outside, -outside));

	eastWall.m_Faces[0][1] = Quad(vector3F(-inside, inside, outside), vector3F(-inside, -inside, outside), vector3F(-inside, -inside, inside), vector3F(-inside, inside, inside));
	eastWall.m_Faces[1][1] = Quad(vector3F(-inside, inside, inside), vector3F(-inside, -inside, inside), vector3F(-inside, -inside, -inside), vector3F(-inside, inside, -inside));
	eastWall.m_Faces[2][1] = Quad(vector3F(-inside, inside, -inside), vector3F(-inside, -inside, -inside), vector3F(-inside, -inside, -outside), vector3F(-inside, inside, -outside));

	eastWall.m_Faces[0][2] = Quad(vector3F(-inside, -inside, outside), vector3F(-inside, -outside, outside), vector3F(-inside, -outside, inside), vector3F(-inside, -inside, inside));
	eastWall.m_Faces[1][2] = Quad(vector3F(-inside, -inside, inside), vector3F(-inside, -outside, inside), vector3F(-inside, -outside, -inside), vector3F(-inside, -inside, -inside));
	eastWall.m_Faces[2][2] = Quad(vector3F(-inside, -inside, -inside), vector3F(-inside, -outside, -inside), vector3F(-inside, -outside, -outside), vector3F(-inside, -inside, -outside));

	eastWall.m_Checks[0][0] = vector3B(0, 1, 1); //Up Forward
	eastWall.m_Checks[1][0] = vector3B(0, 1, 0); //Up
	eastWall.m_Checks[2][0] = vector3B(0, 1, -1); //Up Back

	eastWall.m_Checks[0][1] = vector3B(0, 0, 1); //Forward
	eastWall.m_Checks[1][1] = vector3B(-1, 0, 0); //East
	eastWall.m_Checks[2][1] = vector3B(0, 0, -1); //Back

	eastWall.m_Checks[0][2] = vector3B(0, -1, 1); //Down Forward
	eastWall.m_Checks[1][2] = vector3B(0, -1, 0); //Down
	eastWall.m_Checks[2][2] = vector3B(0, -1, -1); //Down Back

	eastWall.m_SubstitutionDirections[0] = UP; //[1][0]
	eastWall.m_SubstitutionChecks[0][0] = vector2I(0, 0);
	eastWall.m_SubstitutionChecks[0][1] = vector2I(0, 1);
	eastWall.m_SubstitutionChecks[0][2] = vector2I(0, 2);

	eastWall.m_SubstitutionDirections[1] = FORWARD; //[0][1]
	eastWall.m_SubstitutionChecks[1][0] = vector2I(0, 0);
	eastWall.m_SubstitutionChecks[1][1] = vector2I(0, 1);
	eastWall.m_SubstitutionChecks[1][2] = vector2I(0, 2);

	eastWall.m_SubstitutionDirections[2] = DOWN; //[1][2]
	eastWall.m_SubstitutionChecks[2][0] = vector2I(2, 0);
	eastWall.m_SubstitutionChecks[2][1] = vector2I(2, 1);
	eastWall.m_SubstitutionChecks[2][2] = vector2I(2, 2);

	eastWall.m_SubstitutionDirections[3] = BACKWARD; //[2][1]
	eastWall.m_SubstitutionChecks[3][0] = vector2I(2, 0);
	eastWall.m_SubstitutionChecks[3][1] = vector2I(2, 1);
	eastWall.m_SubstitutionChecks[3][2] = vector2I(2, 2);
	//-------------------------------------------------------------------------

	vector<insideCubeFace> faces = { ceiling, floor, northWall, southWall, westWall, eastWall };

	vector<MaterialVertex> verticesVector = vector<MaterialVertex>();
	vector<Material> materialVector = vector<Material>();
	materialVector.push_back(Material("primary", vector3F(0.8f, 0.0f, 0.5f), 2, 1.0));
	materialVector.push_back(Material("second", vector3F(0.8f, 0.8f, 0.8f), 2, 1.0));
	materialVector.push_back(Material("3", vector3F(0.0f, 0.8f, 0.8f), 2, 1.0));
	materialVector.push_back(Material("4", vector3F(0.0f, 0.8f, 0.0f), 2, 1.0));
	materialVector.push_back(Material("5", vector3F(0.8f, 0.8f, 0.0f), 2, 1.0));

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		vector3B pos = it->first;
		vector3F offset = (vector3F)pos * 3.0f;

		for (Node node : it->second.getInternalNodePoints())
		{
			int direction = node.m_direction;
			insideCubeFace face = faces[direction];

			if (!hasInternalNode(pos + face.m_Checks[1][1], flipDirection(direction)))
			{
				PushQuad(verticesVector, face.m_Faces[1][1], face.m_Normal, 1, offset);

				if (hasInternalNode(pos + face.m_Checks[1][0], face.m_SubstitutionDirections[0]))
				{
					PushQuad(verticesVector, face.m_Faces[1][0], face.m_Normal, 0, offset);
				}

				if (hasInternalNode(pos + face.m_Checks[0][1], face.m_SubstitutionDirections[1]))
				{
					PushQuad(verticesVector, face.m_Faces[0][1], face.m_Normal, 0, offset);
				}

				if (hasInternalNode(pos + face.m_Checks[1][2], face.m_SubstitutionDirections[2]))
				{
					PushQuad(verticesVector, face.m_Faces[1][2], face.m_Normal, 0, offset);
				}

				if (hasInternalNode(pos + face.m_Checks[2][1], face.m_SubstitutionDirections[3]))
				{
					PushQuad(verticesVector, face.m_Faces[2][1], face.m_Normal, 0, offset);
				}

				//Top Left Corner
				if (hasInternalNode(pos + face.m_Checks[0][0], direction) && hasInternalNode(pos + face.m_Checks[1][0], face.m_SubstitutionDirections[0]) && hasInternalNode(pos + face.m_Checks[0][1], face.m_SubstitutionDirections[1]))
				{
					PushQuad(verticesVector, face.m_Faces[0][0], face.m_Normal, 0, offset);
				}

				//Top Right Corner
				if (hasInternalNode(pos + face.m_Checks[2][0], direction) && hasInternalNode(pos + face.m_Checks[1][0], face.m_SubstitutionDirections[0]) && hasInternalNode(pos + face.m_Checks[2][1], face.m_SubstitutionDirections[3]))
				{
					PushQuad(verticesVector, face.m_Faces[2][0], face.m_Normal, 0, offset);
				}

				//Bottom Left Corner
				if (hasInternalNode(pos + face.m_Checks[0][2], direction) && hasInternalNode(pos + face.m_Checks[1][2], face.m_SubstitutionDirections[2]) && hasInternalNode(pos + face.m_Checks[0][1], face.m_SubstitutionDirections[1]))
				{
					PushQuad(verticesVector, face.m_Faces[0][2], face.m_Normal, 0, offset);
				}

				//Bottom Right Corner
				if (hasInternalNode(pos + face.m_Checks[2][2], direction) && hasInternalNode(pos + face.m_Checks[1][2], face.m_SubstitutionDirections[2]) && hasInternalNode(pos + face.m_Checks[2][1], face.m_SubstitutionDirections[3]))
				{
					PushQuad(verticesVector, face.m_Faces[2][2], face.m_Normal, 0, offset);
				}
			}
			else
			{
				if (hasInternalNode(pos + face.m_Checks[1][0], direction) && !hasInternalNode(pos + face.m_Checks[1][0] + face.m_Checks[1][1], direction))
				{
					PushQuad(verticesVector, face.m_Faces[1][0], face.m_Normal, 2, offset);
				}

				if (hasInternalNode(pos + face.m_Checks[1][2], direction) && !hasInternalNode(pos + face.m_Checks[1][2] + face.m_Checks[1][1], direction))
				{
					PushQuad(verticesVector, face.m_Faces[1][2], face.m_Normal, 2, offset);
				}

				if (hasInternalNode(pos + face.m_Checks[0][1], direction) && !hasInternalNode(pos + face.m_Checks[0][1] + face.m_Checks[1][1], direction))
				{
					PushQuad(verticesVector, face.m_Faces[0][1], face.m_Normal, 2, offset);
				}

				if (hasInternalNode(pos + face.m_Checks[2][1], direction) && !hasInternalNode(pos + face.m_Checks[2][1] + face.m_Checks[1][1], direction))
				{
					PushQuad(verticesVector, face.m_Faces[2][1], face.m_Normal, 2, offset);
				}

				//Top Left Corner
				if (hasInternalNode(pos + face.m_Checks[0][0], direction) && hasInternalNode(pos + face.m_Checks[1][0], direction) && hasInternalNode(pos + face.m_Checks[0][1], direction) && (!hasInternalNode(pos + face.m_Checks[1][0] + face.m_Checks[1][1], direction) || !hasInternalNode(pos + face.m_Checks[0][1] + face.m_Checks[1][1], direction)))
				{
					PushQuad(verticesVector, face.m_Faces[0][0], face.m_Normal, 3, offset);
				}

				//Top Right Corner
				if (hasInternalNode(pos + face.m_Checks[2][0], direction) && hasInternalNode(pos + face.m_Checks[1][0], direction) && hasInternalNode(pos + face.m_Checks[2][1], direction) && (!hasInternalNode(pos + face.m_Checks[1][0] + face.m_Checks[1][1], direction) || !hasInternalNode(pos + face.m_Checks[2][1] + face.m_Checks[1][1], direction)))
				{
					PushQuad(verticesVector, face.m_Faces[2][0], face.m_Normal, 3, offset);
				}

				//Bottom Left Corner
				if (hasInternalNode(pos + face.m_Checks[0][2], direction) && hasInternalNode(pos + face.m_Checks[1][2], direction) && hasInternalNode(pos + face.m_Checks[0][1], direction) && (!hasInternalNode(pos + face.m_Checks[1][2] + face.m_Checks[1][1], direction) || !hasInternalNode(pos + face.m_Checks[0][1] + face.m_Checks[1][1], direction)))
				{
					PushQuad(verticesVector, face.m_Faces[0][2], face.m_Normal, 3, offset);
				}

				//Bottom Right Corner
				if (hasInternalNode(pos + face.m_Checks[2][2], direction) && hasInternalNode(pos + face.m_Checks[1][2], direction) && hasInternalNode(pos + face.m_Checks[2][1], direction) && (!hasInternalNode(pos + face.m_Checks[1][2] + face.m_Checks[1][1], direction) || !hasInternalNode(pos + face.m_Checks[2][1] + face.m_Checks[1][1], direction)))
				{
					PushQuad(verticesVector, face.m_Faces[2][2], face.m_Normal, 3, offset);
				}

				//Cube Case Top Left
				if (hasInternalNode(pos + face.m_Checks[0][0], direction) && !hasInternalNode(pos + face.m_Checks[0][0] + face.m_Checks[1][1], direction) && hasInternalNode(pos + face.m_Checks[1][0], direction) && hasInternalNode(pos + face.m_Checks[1][0] + face.m_Checks[1][1], direction) && hasInternalNode(pos + face.m_Checks[0][1], direction) && hasInternalNode(pos + face.m_Checks[0][1] + face.m_Checks[1][1], direction))
				{
					PushQuad(verticesVector, face.m_Faces[0][0], face.m_Normal, 3, offset);
				}

				//Cube Case Top Right
				if (hasInternalNode(pos + face.m_Checks[2][0], direction) && !hasInternalNode(pos + face.m_Checks[2][0] + face.m_Checks[1][1], direction) && hasInternalNode(pos + face.m_Checks[1][0], direction) && hasInternalNode(pos + face.m_Checks[1][0] + face.m_Checks[1][1], direction) && hasInternalNode(pos + face.m_Checks[2][1], direction) && hasInternalNode(pos + face.m_Checks[2][1] + face.m_Checks[1][1], direction))
				{
					PushQuad(verticesVector, face.m_Faces[2][0], face.m_Normal, 3, offset);
				}

				//Cube Case Bottom Right
				if (hasInternalNode(pos + face.m_Checks[2][2], direction) && !hasInternalNode(pos + face.m_Checks[2][2] + face.m_Checks[1][1], direction) && hasInternalNode(pos + face.m_Checks[1][2], direction) && hasInternalNode(pos + face.m_Checks[1][2] + face.m_Checks[1][1], direction) && hasInternalNode(pos + face.m_Checks[2][1], direction) && hasInternalNode(pos + face.m_Checks[2][1] + face.m_Checks[1][1], direction))
				{
					PushQuad(verticesVector, face.m_Faces[2][2], face.m_Normal, 3, offset);
				}

				//Cube Case Bottom Left
				if (hasInternalNode(pos + face.m_Checks[0][2], direction) && !hasInternalNode(pos + face.m_Checks[0][2] + face.m_Checks[1][1], direction) && hasInternalNode(pos + face.m_Checks[1][2], direction) && hasInternalNode(pos + face.m_Checks[1][2] + face.m_Checks[1][1], direction) && hasInternalNode(pos + face.m_Checks[0][1], direction) && hasInternalNode(pos + face.m_Checks[0][1] + face.m_Checks[1][1], direction))
				{
					PushQuad(verticesVector, face.m_Faces[0][2], face.m_Normal, 3, offset);
				}


				//Model Node case
				for (int i = 0; i < 4; i++)
				{
					DIRECTIONS checkDirection = face.m_SubstitutionDirections[i];

					if (!hasInternalNode(pos, checkDirection))
					{
						insideCubeFace face1 = faces[checkDirection];

						vector2I value = face1.m_SubstitutionChecks[i][1];
						PushQuad(verticesVector, face1.m_Faces[value.x][value.y], face1.m_Normal, 4, offset);

						//Corner
						/*if (hasInternalNode(pos + face.m_Checks[0][0], direction) && hasInternalNode(pos + face.m_Checks[1][0], face.m_SubstitutionDirections[0]) && hasInternalNode(pos + face.m_Checks[0][1], face.m_SubstitutionDirections[1]))
						{
							PushQuad(verticesVector, face.m_Faces[0][0], face.m_Normal, 0, offset);
						}

						//Corner
						if (hasInternalNode(pos + face.m_Checks[2][0], direction) && hasInternalNode(pos + face.m_Checks[1][0], face.m_SubstitutionDirections[0]) && hasInternalNode(pos + face.m_Checks[2][1], face.m_SubstitutionDirections[3]))
						{
							PushQuad(verticesVector, face.m_Faces[2][0], face.m_Normal, 0, offset);
						}*/

					}
				}

			}
		}
	}

	if (verticesVector.size() == 0)
	{
		return nullptr;
	}

	return new MaterialMesh(verticesVector, materialVector);
}