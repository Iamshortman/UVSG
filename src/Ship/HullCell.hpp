#ifndef HULLCELL_HPP
#define HULLCELL_HPP

#include "Ship/ShipCell.hpp"
#include "Ship/ShipPart.hpp"
#include "Ship/ShipComponent.hpp"

class HullCell : public ShipCell
{
public:
	//Colors for each Material Face
	Material* outsideFaceMaterial;
	Material* insideFaceMaterial;
	double cellMass = 200.0;

	//ShipParts
	ShipPart* parts[6];

	virtual bool doesPartAffectOutside(int direction)
	{
		if (parts[direction] == nullptr)
		{
			return false;
		}

		return parts[direction]->affectOutsideFace;
	};

	virtual bool doesPartAffectInside(int direction)
	{
		if (parts[direction] == nullptr)
		{
			return false;
		}

		return parts[direction]->affectInsideFace;
	};

	virtual double getMass()
	{
		return this->cellMass;
	};

	virtual void addCellToMesh(ShipComponent* ship, vector3S pos, float cubeSize, vector<MaterialVertex>& vertices, vector<Material>& materials)
	{
		vector3F offset = (vector3F)pos * cubeSize;

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
	};

	void addCollisionShape(vector3D pos, btCompoundShape* shape)
	{
		shape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), toBtVec3(pos)), new btBoxShape(btVector3(0.5, 0.5, 0.5)));
	}
};

#endif //HULLCELL_HPP