#ifndef UVSG_HPP
#define UVSG_HPP

#include "entityxInclude.hpp"
#include "RenderingManager.hpp"
#include "PhysicsWorld.hpp"
#include "PhysxWorld.hpp"

class UVSG
{

public:
	UVSG();
	virtual ~UVSG();
	EntityX entitySystem;
	void update(double deltaTime);
	void exitGame();
	const bool getShouldClose();

	static UVSG* getInstance();

	RenderingManager* renderingManager;
	PhysicsWorld* physicsWorld;
	PhysxWorld* physxWorld;

	SDL_Haptic *haptic = NULL;
	SDL_Joystick *joystick = NULL;
	SDL_GameController *controller = NULL;

protected:

private:
	static UVSG* instance;
	bool shouldClose = false;
};

#endif // UVSG_HPP