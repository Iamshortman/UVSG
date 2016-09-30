#ifndef GAMEPADBUTTON_HPP
#define GAMEPADBUTTON_HPP

#include "InputButton.hpp"

class KeyboardButton : public InputButton
{
public:
	KeyboardButton(int button);
	virtual void update(double deltaTime);
	virtual bool isButtonDown();
	virtual bool isButtonPressed();
	virtual bool isButtonDoublePressed();

private:
	int m_button;

	bool m_buttonValue = 0;
	bool m_buttonValuePrev = 0;
};

#endif //GAMEPADBUTTON_HPP