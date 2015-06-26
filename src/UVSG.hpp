#ifndef UVSG_HPP
#define UVSG_HPP

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "RenderingManager.hpp"

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

	RenderingManager renderingManager;

protected:

private:
	static UVSG* instance;
	bool shouldClose = false;
};

#endif // UVSG_HPP