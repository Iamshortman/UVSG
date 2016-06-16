#ifndef UVSG_HPP
#define UVSG_HPP

#include "Rendering/RenderingManager.hpp"
#include "EntityxInclude.hpp"
#include "Scene/Scene.hpp"
#include "Ship/ShipCellDictionary.hpp"

class UVSG
{

public:
	UVSG();
	virtual ~UVSG();

	void update(double deltaTime);
	void exitGame();
	const bool getShouldClose();

	static UVSG* getInstance();

	RenderingManager* renderingManager;
	Scene* currentScene;
	ShipCellDictionary* shipCellDictionary;

protected:

private:
	static UVSG* instance;
	bool shouldClose = false;
};

#endif // UVSG_HPP
