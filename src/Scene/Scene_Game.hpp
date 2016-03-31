#ifndef SCENE_GAME_HPP
#define SCENE_GAME_HPP

#include "Scene/Scene.hpp"
#include "World/World.hpp"
#include "Rendering/RenderingManager.hpp"

class Scene_Game : public Scene
{
public:
	Scene_Game()
	{
		this->baseWorld = new World();
	};

	virtual ~Scene_Game()
	{
		delete baseWorld;
	};

	virtual void update(double deltaTime)
	{
		baseWorld->updateWorld(deltaTime);
	};

	virtual void render(RenderingManager* manager)
	{
		manager->update(0.0, baseWorld);
	};

	World* baseWorld;
};

#endif //SCENE_GAME_HPP