#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "openGL.hpp"

#include <SDL2/SDL.h>
#include <SDL/SDL_events.h>

#include <string>

typedef std::string AE_String;

using namespace std;

class Screen
{
	public:
		Screen(int width = 640 , int height = 480, AE_String windowTitle = "");
		virtual ~Screen();
		const AE_String getTitleString();
		void setTitleString(AE_String title);
		void closeScreen();
		void HandleEvent(SDL_Event& e);
		void updateBuffer();
		void clearBuffer();
		void setBufferClearColor(GLclampf red, GLclampf green, GLclampf blue,
GLclampf alpha);
        void resizeScreen(int width, int height);
        void getScreenSize(int &width, int &height);
        SDL_Window *window;

	private:

		SDL_GLContext glcontext;
		void initGL();
};

#endif
