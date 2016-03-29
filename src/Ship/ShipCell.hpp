#ifndef SHIPCELL_HPP
#define SHIPCELL_HPP

#include "Rendering/Material.hpp"
#include "Ship/ShipPart.hpp"

class ShipCell
{
public:
	//Colors for each Material Face
	Material* outsideFaceMaterial;
	Material* insideFaceMaterial;
	double cellMass = 0.0;

	//ShipParts
	ShipPart* parts[6];

	bool doesPartAffectOutside(int direction)
	{
		if (parts[direction] == nullptr)
		{
			return false;
		}

		return parts[direction]->affectOutsideFace;
	};

	bool doesPartAffectInside(int direction)
	{
		if (parts[direction] == nullptr)
		{
			return false;
		}

		return parts[direction]->affectInsideFace;
	};
};

#endif //SHIPCELL_HPP