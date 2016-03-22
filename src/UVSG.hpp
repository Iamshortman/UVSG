#ifndef UVSG_HPP
#define UVSG_HPP

#include "Rendering/RenderingManager.hpp"
#include "World/World.hpp"
#include "EntityxInclude.hpp"
#include "Rendering/Renderer.hpp"

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
	World* baseWorld;

	Renderer* m_renderer;

protected:

private:
	static UVSG* instance;
	bool shouldClose = false;
};

#endif // UVSG_HPP
