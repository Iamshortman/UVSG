#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "openGL.hpp"

#include <SDL2/SDL.h>
#include <SDL/SDL_events.h>

#include <string>

using namespace std;

class Window
{
	public:
		Window(int width = 640 , int height = 480, string windowTitle = "");
		virtual ~Window();
		const string getTitleString();
		void setTitleString(string title);
		void closeWindow();
		void HandleEvent(SDL_Event& e);
		void updateBuffer();
		void clearBuffer();
		void setBufferClearColor(GLclampf red, GLclampf green, GLclampf blue,
GLclampf alpha);
        void resizeWindow(int width, int height);
        void getWindowSize(int &width, int &height);
        SDL_Window *window;
        bool isWindowActive();
        void setMousePos(int x, int y);
        void getMousePos(int &x, int &y);

	private:
        bool keyboardFocus;
		SDL_GLContext glcontext;
		void initGL();
};

#endif
