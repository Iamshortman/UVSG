#ifndef SHIPMESHCOMPONENT_HPP
#define SHIPMESHCOMPONENT_HPP

#include "Components/Component.hpp"
#include "Renderable.hpp"
#include <vector>

using std::vector;

class ShipMeshComponent : public Component
{
public:
	vector<Model*> outsideModels;
	vector<Model*> insideModels;
};


#endif //SHIPMESHCOMPONENT_HPP