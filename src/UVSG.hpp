#ifndef UVSG_HPP
#define UVSG_HPP

#include "Rendering/RenderingManager.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "EntityxInclude.hpp"

struct Bolt
{

};

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
	Entity player;
	Entity m_camera;
	
	void tempCreatePlasmaBolt(vector3D pos, quaternionD direction);
	Model* boltModel;

protected:

private:
	static UVSG* instance;
	bool shouldClose = false;
};

#endif // UVSG_HPP
