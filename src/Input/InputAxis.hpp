#ifndef INPUTAXIS_HPP
#define INPUTAXIS_HPP

class InputAxis
{
public:
	virtual void update(double deltaTime) = 0;
	virtual double getAxis() = 0;
	
	virtual void setDeadzone(double deadzone) = 0;
	virtual void setInverted(bool inverted) = 0;

	virtual double getDeadzone() = 0;
	virtual bool getInverted() = 0;
};

#endif //INPUTAXIS_HPP