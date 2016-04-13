#ifndef ENGINECELL_HPP
#define ENGINECELL_HPP

#include "Ship/ShipCell.hpp"
#include "Ship/ShipPart.hpp"
#include "Ship/ShipComponent.hpp"

class EngineCell : public ShipCell
{
private:
	Mesh* mesh = nullptr;

public:
	EngineCell()
	{
		if (mesh == nullptr)
		{
			mesh = loadMaterialMeshFromFile("res/", "Basic_Engine.obj");
		}
	};

	~EngineCell()
	{
		delete mesh;
	};

	double cellMass = 450.0;
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
			vector3S(0, 0, -1)
		}; 
	
	};

	void addCollisionShape(vector3D pos, btCompoundShape* shape)
	{
		shape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), toBtVec3(pos + vector3D(0, 0, -0.375))), new btBoxShape(btVector3(0.5, 0.5, 0.875)));
	}
};

#endif //ENGINECELL_HPP