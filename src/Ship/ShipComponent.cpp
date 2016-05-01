#include "Ship/ShipComponent.hpp"

#include "Util.hpp"

ShipComponent::ShipComponent()
{
	m_shipCells = Ship_Map();
}

void ShipComponent::addCell(ShipCellData cell)
{
	m_shipCells[cell.m_position] = cell;
}

void ShipComponent::removeCell(vector3S pos)
{
	if (m_shipCells.find(pos) != m_shipCells.end())
	{
		m_shipCells.erase(pos);
		return;
	}

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		if (it->second.isCellAtPoint(pos))
		{
			m_shipCells.erase(it->first);
			return;
		}
	}
}

ShipCellData ShipComponent::getCell(vector3S pos)
{
	if (m_shipCells.find(pos) != m_shipCells.end())
	{
		return m_shipCells[pos];
	}

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		if (it->second.isCellAtPoint(pos))
		{
			return it->second;
		}
	}

	return ShipCellData();
}

bool ShipComponent::hasCellAtPos(vector3S pos)
{
	if (m_shipCells.find(pos) != m_shipCells.end())
	{
		return true;
	}

	for (auto it = m_shipCells.begin(); it != m_shipCells.end(); ++it)
	{
		if (it->second.isCellAtPoint(pos))
		{
			return true;
		}
	}

	return false;
}

bool ShipComponent::hasNode(vector3S pos, int direction)
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

bool ShipComponent::canPlaceCell(vector3S pos, ShipCellData& cell)
{
	for (vector3S point : cell.getCellPoints())
	{
		if (hasCellAtPos(point))
		{
			return false;
		}
	}

	return true;
}

Mesh* ShipComponent::genOutsideMesh()
{
	float cubeSize = 3.0f;
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

	vector3S cellOffset[] =
	{
		vector3S(0, 1, 0),
		vector3S(0, -1, 0),
		vector3S(0, 0, 1),
		vector3S(0, 0, -1),
		vector3S(1, 0, 0),
		vector3S(-1, 0, 0),
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