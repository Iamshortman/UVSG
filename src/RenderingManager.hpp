#ifndef RENDERINGMANAGER_HPP
#define RENDERINGMANAGER_HPP

#include <entityx\entityx.h>

class RenderingManager
{

public:
	RenderingManager();
	virtual ~RenderingManager();
	void update(entityx::EntityX ex, float deltaTime);

	static RenderingManager* getInstance();

protected:

private:
	static RenderingManager* instance;
};

#endif // RENDERINGMANAGER_HPP