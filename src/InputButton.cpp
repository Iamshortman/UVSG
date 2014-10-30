#include "InputButton.hpp"

//Non Class function
bool contains(const std::deque<int> &buttons, const int &value)
{
    return std::find(buttons.begin(), buttons.end(), value) != buttons.end();
}

InputButton::InputButton()
{

    //Enables JoyStick for use
    SDL_Init(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    //TODO Actively load joysticks.
    SDL_JoystickOpen(0);
}

void InputButton::update()
{
    //state = SDL_GetKeyboardState(NULL);
}

//Used to update which buttons are down
void InputButton::HandleEvent(SDL_Event& e)
{
    //Tells the system keys were down last loop
    keyboardLast = keyboard;
    mouseLast = mouse;

    //clears the deque for this loop
    keyboard = std::vector<int>();
    mouse = std::deque<int>();

    /*if(e.type == SDL_KEYDOWN)
    {
        int button = e.key.keysym.sym;
        cout << "Code: " << button << endl;
        keyboard.push_back(button);
    }
    else*/ if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        int button = e.button.button;
        mouse.push_back(button);
    }
    else if(e.type == SDL_JOYBUTTONDOWN)
    {
        printf("JoyStickButton Down: %i\n", e.jbutton.button);
    }
}

bool InputButton::isKeyboardButtonDown(int button)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[button])
    {
        return true;
    }

    return false;
}

bool InputButton::isMouseButtonDown(int key)
{
    //If the key is in the deque then it is down currently.
    return contains(mouse, key);
}
