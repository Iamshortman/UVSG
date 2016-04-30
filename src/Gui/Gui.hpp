#ifndef GUI_HPP
#define GUI_HPP

#include <hash_map>
#include <String>

#include "GuiComponent.hpp"

class Gui
{

public:
	Gui();
	~Gui();

	void renderGui(int windowWidth, int windowHeight);
	void AddComponent(std::string name, GuiComponent* component);

private:
	std::hash_map<std::string, GuiComponent*> components;

};

#endif //GUI_HPP