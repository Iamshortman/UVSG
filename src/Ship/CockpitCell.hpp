#ifndef COCKPITCELL_HPP
#define COCKPITCELL_HPP

#include "Ship/ShipCell.hpp"
#include "Ship/ShipPart.hpp"
#include "Ship/ShipComponent.hpp"

class CockpitCell : public ShipCell
{
private:
	Mesh* mesh = nullptr;

public:
	CockpitCell()
	{
		if (mesh == nullptr)
		{
			mesh = loadMaterialMeshFromFile("res/", "Cockpit.obj");
		}
	};

	~CockpitCell()
	{
		delete mesh;
	};

	double cellMass = 500.0;
	virtual bool doesPartAffectOutside(int direction)
	{
		return true;
	};

	virtual bool doesPartAffectInside(int direction)
	{
			return false;
	};

	virtual double getMass()
	{
		return this->cellMass;
	};

	virtual void addCellToMesh(ShipComponent* ship, vector3S pos, float cubeSize, vector<MaterialVertex>& vertices, vector<Material>& materials)
	{

	};

	virtual Mesh* getMesh()
	{
		return mesh;
	};

	virtual vector<vector3S> getCellsCovered()
	{ 
		return { 
			vector3S(0, 0, 0),
			vector3S(0, 0, 1),
			vector3S(0, 1, 1),
			vector3S(0, 1, 0)
		}; 
	
	};

	void addCollisionShape(vector3D pos, btCompoundShape* shape)
	{
		shape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), toBtVec3(pos + vector3D(0, 0.1875, 0.5))), new btBoxShape(btVector3(0.5, 0.6875, 1.0)));
	}
};

#endif //COCKPITCELL_HPP