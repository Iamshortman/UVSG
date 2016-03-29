#ifndef UVSG_HPP
#define UVSG_HPP

#include "Rendering/RenderingManager.hpp"
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


	//Ship Editor Temp Stuff
	SDL_GameController* controller;
	vector3D camOrigin = vector3D(0.0);
	double cameraDistance = 10.0;
	quaternionD cameraRot;
	Model* CusorModel;

protected:

private:
	static UVSG* instance;
	bool shouldClose = false;
};

#endif // UVSG_HPP
