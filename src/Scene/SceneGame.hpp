#ifndef SCENE_GAME_HPP
#define SCENE_GAME_HPP

#include "Scene/Scene.hpp"
#include "World/World.hpp"
#include "Rendering/Renderer.hpp"

class Scene_Game
{
public:
	Scene_Game()
	{
		this->baseWorld = new World();
	};

	virtual ~Scene_Game()
	{
	
	};

	virtual void update(double deltaTime)
	{

	};

	World* baseWorld;
	Renderer* m_renderer;
};

#endif //SCENE_GAME_HPP