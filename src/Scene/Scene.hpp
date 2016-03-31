#ifndef SCENE_HPP
#define SCENE_HPP

class Scene
{
public:
	Scene(){};
	virtual ~Scene(){};
	virtual void update(double deltaTime) = 0;
	virtual void render(RenderingManager* manager) = 0;
};

#endif //SCENE_HPP