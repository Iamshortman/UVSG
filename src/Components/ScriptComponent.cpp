#include "ScriptComponent.hpp"

ScriptComponent::ScriptComponent()
{
	/*chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
	chai.add(chaiscript::fun(&flipDirection), "flip");

	int flipped = chai.eval<int>("flip(1)");
	printf("int: %i\n", flipped);*/
}

ScriptComponent::~ScriptComponent()
{

}

void ScriptComponent::update(double deltaTime)
{
}