#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "imgui_impl_sdl.h"

#include "PanelConfiguration.h"
#include "PanelAbout.h"
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
	//if (ImGui::BeginMainMenuBar()) 
	//{
	//	if (ImGui::BeginMenu("File")) 
	//	{
	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu("View"))
	//	{
	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu("Help"))
	//	{
	//		ImGui::EndMenu();
	//	}

	//	ImGui::EndMainMenuBar();
	//}

	if (ImGui::Begin("Panels", NULL, ImGuiWindowFlags_NoScrollbar))
	{
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