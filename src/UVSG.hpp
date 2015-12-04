#ifndef UVSG_HPP
#define UVSG_HPP

#include "RenderingManager.hpp"
#include "PhysicsWorld.hpp"
#include "EntityxInclude.hpp"

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
	PhysicsWorld* physicsWorld;

	SDL_Haptic *haptic = NULL;
	SDL_Joystick *joystick = NULL;
	SDL_GameController *controller = NULL;

	EntityX entitySystem;

protected:

private:
	static UVSG* instance;
	bool shouldClose = false;
};

#endif // UVSG_HPP