#include "Window.hpp"

Window::Window(int width, int height, string windowTitle)
{
	SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Create an application window with the following settings:
    window = SDL_CreateWindow(
        windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
    );

    if( window == NULL )
    {
        exit(1);
    }

	// Create an OpenGL context associated with the window.
	glcontext = SDL_GL_CreateContext(window);

    	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(1);
	}

	initGL();
	resizeWindow(width, height);
}

void Window::initGL()
{
    // init OpenGL here
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D);
    //glCullFace(GL_BACK);
}

void Window::resizeWindow(int width, int height)
{
    if (height == 0) // Prevent A Divide By Zero By
    {
        height = 1;// Making Height Equal One
    }

    glViewport(0, 0, width, height); // Reset The Current Viewport
}

void Window::getWindowSize(int &width, int &height)
{
    SDL_GetWindowSize(window, &width, &height);
}

void Window::HandleEvent(SDL_Event& e)
{
     if( e.type == SDL_WINDOWEVENT )
     {
        if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
        int newWidth = e.window.data1;
        int newHeight = e.window.data2;
        this->resizeWindow(newWidth, newHeight);
        }
        else if(e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
        {
            keyboardFocus = true;
        }
        else if(e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        {
            keyboardFocus = false;
        }

     }
}

void Window::updateBuffer()
{
	SDL_GL_SwapWindow(window);
}

void Window::setBufferClearColor(GLclampf red, GLclampf green, GLclampf blue,
GLclampf alpha)
{
	 glClearColor(red, green, blue, alpha);
}

void Window::clearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setTitleString(string title)
{
	SDL_SetWindowTitle(window, title.c_str());
}

const string Window::getTitleString()
{
	return SDL_GetWindowTitle(window);
}

Window::~Window()
{
    closeWindow();
}

bool Window::isWindowActive()
{
    return keyboardFocus;
}

void Window::setMousePos(int x, int y)
{
    SDL_WarpMouseInWindow(window, x, y);
}

void Window::getMousePos(int &x, int &y)
{
     SDL_GetMouseState(&x, &y);
}

void Window::closeWindow()
{
	SDL_DestroyWindow(window);
}
