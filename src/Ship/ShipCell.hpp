#ifndef SHIPCELL_HPP
#define SHIPCELL_HPP

#include "Rendering/MaterialMesh.hpp"
#include "Ship/ShipPart.hpp"
#include "Directions.hpp"
#include "btBulletCollisionCommon.h"

#include <vector>
#include <array>
using std::vector;
using std::array;

//PrototypeClass
class ShipComponent;

class ShipCell
{
public:
	virtual bool doesPartAffectOutside(int direction) = 0;
	virtual bool doesPartAffectInside(int direction) = 0;
	virtual double getMass() = 0;

	//Need to make the following functions static somehow.
	virtual void addCellToMesh(ShipComponent* ship, vector3S pos, float cubeSize, vector<MaterialVertex>& vertices, vector<Material>& materials){};
	virtual Mesh* getMesh() { return nullptr; };
	virtual vector<vector3S> getCellsCovered() { return{ vector3S(0) }; };

	virtual void addCollisionShape(vector3D pos, btCompoundShape* shape) {};
};

#endif //SHIPCELL_HPP