#include "Screen.hpp"

Screen::Screen(int width, int height, AE_String windowTitle)
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
	resizeScreen(width, height);
}

void Screen::initGL()
{
    // init OpenGL here
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D);
    glCullFace(GL_BACK);
}

void Screen::resizeScreen(int width, int height)
{
    if (height == 0) // Prevent A Divide By Zero By
    {
        height = 1;// Making Height Equal One
    }

    glViewport(0, 0, width, height); // Reset The Current Viewport
}

void Screen::getScreenSize(int &width, int &height)
{
    SDL_GetWindowSize(window, &width, &height);
}

void Screen::HandleEvent(SDL_Event& e)
{
     if( e.type == SDL_WINDOWEVENT )
     {
        if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
        int newWidth = e.window.data1;
        int newHeight = e.window.data2;
        this->resizeScreen(newWidth, newHeight);
        }
     }
}

void Screen::updateBuffer()
{
	SDL_GL_SwapWindow(window);
}

void Screen::setBufferClearColor(GLclampf red, GLclampf green, GLclampf blue,
GLclampf alpha)
{
	 glClearColor(red, green, blue, alpha);
}

void Screen::clearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::setTitleString(AE_String title)
{
	SDL_SetWindowTitle(window, title.c_str());
}

const AE_String Screen::getTitleString()
{
	AE_String title = SDL_GetWindowTitle(window);
	return title;
}

Screen::~Screen()
{
    closeScreen();
}

void Screen::closeScreen()
{
	SDL_DestroyWindow(window);
}
