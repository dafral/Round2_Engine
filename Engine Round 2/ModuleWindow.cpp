#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	CONSOLELOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		CONSOLELOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			CONSOLELOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	CONSOLELOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

float ModuleWindow::GetScreenWidth()
{
	return width;
}

float ModuleWindow::GetScreenHeight()
{
	return height;
}

float ModuleWindow::GetBrightness()
{
	return SDL_GetWindowBrightness(window);
}

//float ModuleWindow::GetOpacity()
//{
//
//}

void ModuleWindow::SetScreenWidth(float new_width)
{
	width = new_width;
}

void ModuleWindow::SetScreenHeight(float new_height)
{
	height = new_height;
}

void ModuleWindow::SetBrightness(float new_bright)
{
	SDL_SetWindowBrightness(window, new_bright);
}


bool ModuleWindow::GetBordered()
{
	return bordered;
}

bool ModuleWindow::GetFullscreen()
{
	return fullscreen;
}

bool ModuleWindow::GetResizable()
{
	return resizable;
}

void ModuleWindow::SwitchBordered()
{
	bordered = !bordered;
	SDL_SetWindowBordered(window, (SDL_bool)bordered);
}

void ModuleWindow::SwitchFullscreen()
{
	fullscreen = !fullscreen;
	SDL_SetWindowFullscreen(window, fullscreen);
}

void ModuleWindow::SwitchResizable()
{
	resizable = !resizable;
}

int ModuleWindow::GetWidth()
{
	int w;
	SDL_GetWindowSize(window, &w, nullptr);

	return w;
}

int ModuleWindow::GetHeight()
{
	int h;
	SDL_GetWindowSize(window, nullptr, &h);

	return h;
}