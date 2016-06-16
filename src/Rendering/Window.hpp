#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "OpenGL_Include.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include <string>

using namespace std;

class Window
{
	public:
		Window();
		Window(int width, int height, string windowTitle);
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
		void setVsync(int syncRate);
		int getVsync();
		void set3dRendering();
		void set2dRendering();

	private:
        bool keyboardFocus;
		SDL_GLContext glcontext;
		void initGL();

};

#endif
