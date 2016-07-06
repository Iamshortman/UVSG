#ifndef INPUTBUTTON_HPP
#define INPUTBUTTON_HPP

class InputButton
{
public:
	virtual void update(double deltaTime) = 0;
	virtual bool isButtonDown() = 0;
	virtual bool isButtonPressed() = 0;
	virtual bool isButtonDoublePressed() = 0;
};

#endif //INPUTBUTTON_HPP