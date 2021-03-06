#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "ImGui/imgui_impl_sdl.h"

#include "PanelConfiguration.h"
#include "PanelProperties.h"
#include "PanelConsole.h"
#include "PanelScene.h"
#include "PanelHierarchy.h"
#include "PanelGame.h"
#include "PanelTime.h"

//Constructor
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Init()
{
	ImGui_ImplSdl_Init(App->window->window);
	ImGui::InitDock();

	panels.push_back(scene = new PanelScene(true));
	panels.push_back(configuration = new PanelConfiguration(true));
	panels.push_back(properties = new PanelProperties(true));
	panels.push_back(console = new PanelConsole(true));
	panels.push_back(hierarchy = new PanelHierarchy(true));
	panels.push_back(game = new PanelGame(true));
	panels.push_back(time = new PanelTime(true));
	
	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{	

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::Button("Save"))
			{
				App->scene_importer->SaveScene(App->filesystem->scene_path.c_str());
			}

			if(ImGui::Button("Load"))
			{
				App->scene_importer->LoadScene(App->filesystem->scene_path.c_str());
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{		
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::Checkbox("Configuration", App->editor->configuration->isActive()))
			{
				App->editor->configuration->switchActive();
			}
			
			if (ImGui::Checkbox("Properties", App->editor->properties->isActive()))
			{
				App->editor->properties->switchActive();
			}

			if (ImGui::Checkbox("Console", App->editor->console->isActive()))
			{
				App->editor->console->switchActive();
			}

			if (ImGui::Checkbox("Hierarchy", App->editor->hierarchy->isActive()))
			{
				App->editor->console->switchActive();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::BeginMenu("About the engine"))
			{
				ImGui::Text("In order to load a fbx file, drag and drop it on the engine window. You can do the same with textures.");
				ImGui::Text("Remember that the only files supported by the engines are FBX files for the 3D models and DDS/PNG files for the textures.");
				ImGui::Text("You can find some example files inside the 3D Models folder.");
				ImGui::EndMenu();
			}

			if (ImGui::Button("Github Webpage"))
			{
				ShellExecute(0, 0, "https://github.com/dafral/Round2_Engine", 0, 0, SW_SHOW);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Quit"))
		{
			return update_status::UPDATE_STOP;
		}

		ImGui::EndMainMenuBar();
	}

	return UPDATE_CONTINUE;
}
	
bool ModuleEditor::CleanUp()
{
	for (std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		Panel* panel = (*it);

		if (panel != nullptr)
		{
			delete panel;
			panel = nullptr;
		}
	}

	panels.clear();

	return true;
}

void ModuleEditor::Draw()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin("Dock", 0, flags))
	{
		int h_offset = 6;
		int w_offset = 0;
		int w = SDL_GetWindowSurface(App->window->window)->w;
		int h = SDL_GetWindowSurface(App->window->window)->h;

		// Setting position on the screen
		ImGui::SetWindowPos(ImVec2(w_offset, h_offset));
		ImGui::SetWindowSize(ImVec2(w - w_offset, h - h_offset));

		// Dock
		ImGui::BeginDockspace();

		for (std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
		{
			if ((*it)->isActive())
			{ 
				Panel* panel = (*it);
				panel->Draw();
			}
		}

		ImGui::EndDockspace();
	}

	ImGui::End();
	ImGui::Render();
}