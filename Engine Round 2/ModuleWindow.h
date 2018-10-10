#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	float GetScreenWidth();
	float GetScreenHeight();
	float GetBrightness();
	//float GetOpacity();


	void SetScreenWidth(float new_width);
	void SetScreenHeight(float new_height);
	void SetBrightness(float new_bright);
	//void SetOpacity(float new_op);

	bool GetBordered();
	bool GetFullscreen();
	bool GetResizable();

	void SwitchBordered();
	void SwitchFullscreen();
	void SwitchResizable();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:

	float width = 0.0f;
	float height = 0.0f;

public:
	bool bordered = true;
	bool fullscreen = false;
	bool resizable = true;
};

#endif // __ModuleWindow_H__