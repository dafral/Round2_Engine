#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "imgui_impl_sdl.h"

#include "PanelConfiguration.h"
#include "PanelAbout.h"
#include "PanelProperties.h"
#include "PanelConsole.h"
#include "PanelScene.h"

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

	panels.push_back(configuration = new PanelConfiguration(true));
	panels.push_back(about = new PanelAbout(true));
	panels.push_back(properties = new PanelProperties(true));
	panels.push_back(console = new PanelConsole(true));
	panels.push_back(scene = new PanelScene(true));

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			ImGui::Button("Cube");
			ImGui::Button("Sphere");
			ImGui::Button("Plane");
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			bool scene = true;
			
			ImGui::Checkbox("Scene", &scene);

			if (ImGui::Checkbox("Configuration", App->editor->configuration->getActive()))
			{
				App->editor->configuration->switchActive();
				ImGui::CloseCurrentPopup();
			}
			
			if (ImGui::Checkbox("Properties", App->editor->properties->getActive()))
			{
				App->editor->properties->switchActive();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Checkbox("Console", App->editor->console->getActive()))
			{
				App->editor->console->switchActive();
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::Checkbox("About", App->editor->about->getActive()))
			{
				App->editor->about->switchActive();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::BeginMenu("About the engine"))
			{
				ImGui::Text("Prueba");
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

	// Rendering


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

	if (ImGui::Begin("Dock", 0, flags))
	{
		float h_offset = 6.0f;
		float w_offset = 0.0f;

		// Setting position on the screen
		ImGui::SetWindowPos(ImVec2(w_offset, h_offset));
		ImGui::SetWindowSize(ImVec2(App->window->GetScreenWidth() - w_offset, App->window->GetScreenHeight() - h_offset));

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