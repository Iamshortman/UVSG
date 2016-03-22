#ifndef COMPONET_HPP
#define COMPONET_HPP

//Prototype class
class Entity;

class Component
{
private:
	Entity* m_parent;

public:
	Component();
	virtual ~Component();

	virtual void update(double deltaTime);

	Entity* getParent();
	void setParent(Entity* parent);
};

#endif //COMPONET_HPP