#ifndef INPUTBUTTON_HPP
#define INPUTBUTTON_HPP

class InputButton
{
public:
	virtual void update(double deltaTime);
	virtual bool isButtonDown() = 0;
	virtual bool isButtonPressed() = 0;
	virtual bool hasButtonDoublePressed() = 0;
};

#endif //INPUTBUTTON_HPP