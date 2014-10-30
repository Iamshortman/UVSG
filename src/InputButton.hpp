#ifndef INPUTBUTTON_HPP
#define INPUTBUTTON_HPP

#include <SDL2/SDL.h>
#include <SDL/SDL_events.h>
#include <string>
#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;

class InputButton
{
	public:
        InputButton();
        void HandleEvent(SDL_Event& e);
        void update();
        bool isButtonDown(int key, int device);
        bool isKeyboardButtonDown(int key);
        bool isMouseButtonDown(int key);
        bool isGamePadButtonDown(int key, int device);

	private:

        vector<int> keyboard;
        deque<int> mouse;
        deque<int> gamepad;

        vector<int> keyboardLast;
        deque<int> mouseLast;
        deque<int> gamepadLast;

};

#endif
