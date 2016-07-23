#ifndef GAMEPADBUTTON_HPP
#define GAMEPADBUTTON_HPP

#include "InputButton.hpp"
#include <SDL2\SDL_GameController.h>

class GamepadButton : public InputButton
{
public:
	GamepadButton(SDL_GameController* device, SDL_GameControllerButton button);
	virtual void update(double deltaTime);
	virtual bool isButtonDown();
	virtual bool isButtonPressed();
	virtual bool isButtonDoublePressed();

private:
	SDL_GameController* m_device = nullptr;
	SDL_GameControllerButton m_button;

	Uint8 m_buttonValue = 0;
	Uint8 m_buttonValuePrev = 0;
};

#endif //GAMEPADBUTTON_HPP