#include "ShipEditor.hpp"
#include <SDL2/SDL.h>
#include "Util.hpp"

ShipEditor::ShipEditor()
{
	m_cursorPos = vector3S(0);
}

void ShipEditor::Update()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	static Uint8 lastState_UP = 0;
	if (state[SDL_SCANCODE_UP] != 0 && lastState_UP == 0)
	{
		m_cursorPos.z += 1;
	}
	lastState_UP = state[SDL_SCANCODE_UP];

	static Uint8 lastState_DOWN = 0;
	if (state[SDL_SCANCODE_DOWN] != 0 && lastState_DOWN == 0)
	{
		m_cursorPos.z += -1;
	}
	lastState_DOWN = state[SDL_SCANCODE_DOWN];

	static Uint8 lastState_LEFT = 0;
	if (state[SDL_SCANCODE_LEFT] != 0 && lastState_LEFT == 0)
	{
		m_cursorPos.x += 1;
	}
	lastState_LEFT = state[SDL_SCANCODE_LEFT];

	static Uint8 lastState_RIGHT = 0;
	if (state[SDL_SCANCODE_RIGHT] != 0 && lastState_RIGHT == 0)
	{
		m_cursorPos.x += -1;
	}
	lastState_RIGHT = state[SDL_SCANCODE_RIGHT];

	static Uint8 lastState_SPACE = 0;
	if (state[SDL_SCANCODE_SPACE] != 0 && lastState_SPACE == 0)
	{
		if (m_shipCells.find(m_cursorPos) == m_shipCells.end())
		{
			m_shipCells[m_cursorPos] = 1;
		}
		else
		{
			m_shipCells.erase(m_cursorPos);
		}
	}
	lastState_SPACE = state[SDL_SCANCODE_SPACE];
}

ShipEditor::~ShipEditor()
{

}