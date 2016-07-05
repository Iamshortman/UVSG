#ifndef INPUTAXIS_HPP
#define INPUTAXIS_HPP

class InputAxis
{
public:
	virtual void update(double deltaTime) = 0;
	virtual double getAxis() = 0;
	virtual void setDeadzone(double deadzone) = 0;
};

#endif //INPUTAXIS_HPP