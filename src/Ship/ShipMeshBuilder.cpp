#include "Ship/ShipMeshBuilder.hpp"
#include "Ship/Directions.hpp"

Mesh* genOutsideMesh(ShipComponent* ship)
{
	float cubeSize = 2.5f;

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

	vector<MaterialVertex> vertices;
	vector<Material> materials;

	for (auto it = ship->m_shipCells.begin(); it != ship->m_shipCells.end(); ++it)
	{
		vector3S pos = it->first;
		vector3F offset = (vector3F)pos * cubeSize;

		//Top
		if (!ship->hasCell(pos + cellOffset[UP]) && !ship->getCell(pos)->doesPartAffectOutside(UP))
		{
			vertices.push_back({ vertsCube[3] + offset, normals[UP], 0 });
			vertices.push_back({ vertsCube[7] + offset, normals[UP], 0 });
			vertices.push_back({ vertsCube[6] + offset, normals[UP], 0 });

			vertices.push_back({ vertsCube[6] + offset, normals[UP], 0 });
			vertices.push_back({ vertsCube[2] + offset, normals[UP], 0 });
			vertices.push_back({ vertsCube[3] + offset, normals[UP], 0 });
		}

		//Bottom
		if (!ship->hasCell(pos + cellOffset[DOWN]) && !ship->getCell(pos)->doesPartAffectOutside(DOWN))
		{
			vertices.push_back({ vertsCube[5] + offset, normals[DOWN], 0 });
			vertices.push_back({ vertsCube[1] + offset, normals[DOWN], 0 });
			vertices.push_back({ vertsCube[0] + offset, normals[DOWN], 0 });

			vertices.push_back({ vertsCube[0] + offset, normals[DOWN], 0 });
			vertices.push_back({ vertsCube[4] + offset, normals[DOWN], 0 });
			vertices.push_back({ vertsCube[5] + offset, normals[DOWN], 0 });
		}

		//Front
		if (!ship->hasCell(pos + cellOffset[FORWARD]) && !ship->getCell(pos)->doesPartAffectOutside(FORWARD))
		{
			vertices.push_back({ vertsCube[7] + offset, normals[FORWARD], 0 });
			vertices.push_back({ vertsCube[3] + offset, normals[FORWARD], 0 });
			vertices.push_back({ vertsCube[1] + offset, normals[FORWARD], 0 });

			vertices.push_back({ vertsCube[1] + offset, normals[FORWARD], 0 });
			vertices.push_back({ vertsCube[5] + offset, normals[FORWARD], 0 });
			vertices.push_back({ vertsCube[7] + offset, normals[FORWARD], 0 });
		}

		//Back
		if (!ship->hasCell(pos + cellOffset[BACKWARD]) && !ship->getCell(pos)->doesPartAffectOutside(BACKWARD))
		{
			vertices.push_back({ vertsCube[2] + offset, normals[BACKWARD], 0 });
			vertices.push_back({ vertsCube[6] + offset, normals[BACKWARD], 0 });
			vertices.push_back({ vertsCube[4] + offset, normals[BACKWARD], 0 });

			vertices.push_back({ vertsCube[4] + offset, normals[BACKWARD], 0 });
			vertices.push_back({ vertsCube[0] + offset, normals[BACKWARD], 0 });
			vertices.push_back({ vertsCube[2] + offset, normals[BACKWARD], 0 });
		}

		//LEFT
		if (!ship->hasCell(pos + cellOffset[LEFT]) && !ship->getCell(pos)->doesPartAffectOutside(LEFT))
		{
			vertices.push_back({ vertsCube[6] + offset, normals[LEFT], 0 });
			vertices.push_back({ vertsCube[7] + offset, normals[LEFT], 0 });
			vertices.push_back({ vertsCube[5] + offset, normals[LEFT], 0 });

			vertices.push_back({ vertsCube[5] + offset, normals[LEFT], 0 });
			vertices.push_back({ vertsCube[4] + offset, normals[LEFT], 0 });
			vertices.push_back({ vertsCube[6] + offset, normals[LEFT], 0 });
		}

		//Right
		if (!ship->hasCell(pos + cellOffset[RIGHT]) && !ship->getCell(pos)->doesPartAffectOutside(RIGHT))
		{
			vertices.push_back({ vertsCube[3] + offset, normals[RIGHT], 0 });
			vertices.push_back({ vertsCube[2] + offset, normals[RIGHT], 0 });
			vertices.push_back({ vertsCube[0] + offset, normals[RIGHT], 0 });

			vertices.push_back({ vertsCube[0] + offset, normals[RIGHT], 0 });
			vertices.push_back({ vertsCube[1] + offset, normals[RIGHT], 0 });
			vertices.push_back({ vertsCube[3] + offset, normals[RIGHT], 0 });
		}
	}

	Material mat = Material();
	mat.name = "Test";
	mat.diffuse_Color = vector3F(0, 1, 1);
	mat.illum_Value = 2;
	materials.push_back(mat);

	return new MaterialMesh(vertices, materials);
}