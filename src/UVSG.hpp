#ifndef UVSG_HPP
#define UVSG_HPP

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "RenderingManager.hpp"
#include "PhysicsWorld.hpp"

class UVSG
{

public:
	UVSG();
	virtual ~UVSG();
	entityx::EntityX entitySystem;
	void update(float deltaTime);
	void exitGame();
	const bool getShouldClose();

	static UVSG* getInstance();

	RenderingManager* renderingManager;
	PhysicsWorld* physicsWorld;

	SDL_Haptic *haptic = NULL;
	SDL_Joystick *joystick = NULL;
	SDL_GameController *controller = NULL;

protected:

private:
	static UVSG* instance;
	bool shouldClose = false;
};

#endif // UVSG_HPP