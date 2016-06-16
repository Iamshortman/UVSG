#ifndef SCRIPTCOMPONET_HPP
#define SCRIPTCOMPONET_HPP

#include "Component.hpp"
#include "Util.hpp"

#include <string>
//#include <chaiscript/chaiscript.hpp>
//#include <chaiscript/chaiscript_stdlib.hpp>

class ScriptComponent : public Component
{
private:
	//chaiscript::ChaiScript chai;

public:
	ScriptComponent();
	virtual ~ScriptComponent();

	virtual void update(double deltaTime);
};

#endif //SCRIPTCOMPONET_HPP