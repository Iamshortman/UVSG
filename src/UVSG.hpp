#ifndef UVSG_HPP
#define UVSG_HPP

#include <entityx\entityx.h>

class UVSG
{

public:
	UVSG();
	virtual ~UVSG();
	entityx::EntityX entitySystem;
	void update(float deltaTime);

	static UVSG* getInstance();

protected:

private:
	static UVSG* instance;
};

#endif // UVSG_HPP