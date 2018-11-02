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
#include "PanelInspector.h"

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
	/*ImGui::StyleColorsDark();*/

	panels.push_back(scene = new PanelScene(true));
	panels.push_back(configuration = new PanelConfiguration(true));
	panels.push_back(properties = new PanelProperties(true));
	panels.push_back(console = new PanelConsole(true));
	panels.push_back(inspector = new PanelInspector(true));
	
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
		if (ImGui::BeginMenu("Create"))
		{		
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::Checkbox("Configuration", App->editor->configuration->getActive()))
			{
				App->editor->configuration->switchActive();
			}
			
			if (ImGui::Checkbox("Properties", App->editor->properties->getActive()))
			{
				App->editor->properties->switchActive();
			}

			if (ImGui::Checkbox("Console", App->editor->console->getActive()))
			{
				App->editor->console->switchActive();
			}

			if (ImGui::Checkbox("Inspector", App->editor->inspector->getActive()))
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
		float h_offset = 6.0f;
		float w_offset = 0.0f;
		int w = SDL_GetWindowSurface(App->window->window)->w;
		int h = SDL_GetWindowSurface(App->window->window)->h;

		// Setting position on the screen
		ImGui::SetWindowPos(ImVec2(w_offset, h_offset));
		ImGui::SetWindowSize(ImVec2(w - w_offset, h - h_offset));

		// Dock
		ImGui::BeginDockspace();

		for (std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
		{
			if ((*it)->active == true)
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