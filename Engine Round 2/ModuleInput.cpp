#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "PanelHierarchy.h"
#include "PanelScene.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "ImGui/imgui_impl_sdl.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	CONSOLELOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		CONSOLELOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSdl_ProcessEvent(&e);

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			if (App->editor->scene->isHovered()) App->camera->Zoom(mouse_z, dt);
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);

				break;
			}

			// In case if dropped file
			case SDL_DROPFILE: 
			{
				std::string full_path = e.drop.file;

				int dot_pos = full_path.find_last_of(".");
				int bar_pos = full_path.find_last_of("\\");
				int name_length = dot_pos - bar_pos - 1;

				std::string extension = full_path.substr(dot_pos + 1, 3);
				std::string file_name = full_path.substr(bar_pos + 1, name_length);

				if (extension == "fbx" || extension == "FBX") 
				{
					CONSOLELOG("Dropped file: %s", full_path.c_str());
					App->mesh_importer->ImportScene(full_path.c_str(), file_name.c_str());
				}
				else if (extension == "png" || extension == "PNG" || extension == "dds" || extension == "DDS" || extension == "tga" || extension == "TGA")
				{
					CONSOLELOG("Dropped texture: %s", full_path.c_str());

					if (App->editor->hierarchy->GetSelectedGO() != nullptr)
						App->material_importer->Import(full_path.c_str(), App->editor->hierarchy->GetSelectedGO());
					else CONSOLELOG("You MUST select a GameObject to load a texture.");
				}
				else 
				{
					CONSOLELOG("Invalid type of file dropped. This engine only supports FBX and PNG/DDS files!");
				}

				// Free dropped_filedir memory
				SDL_free(e.drop.file);
				break;
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	CONSOLELOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}