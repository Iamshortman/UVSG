#ifndef GUI_HPP
#define GUI_HPP

#include <hash_map>
#include <String>

#include "glmInclude.hpp"
#include "GuiComponent.hpp"

class Gui
{

public:
	Gui();
	~Gui();

	void renderGui(int windowWidth, int windowHeight);
	void AddComponent(std::string name, GuiComponent* component);
	void drawQuad(vector2I pos, vector2I quadSize, vector2I windowSize);

private:
	std::hash_map<std::string, GuiComponent*> components;

};

#endif //GUI_HPP