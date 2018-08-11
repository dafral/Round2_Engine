#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ImGui\imgui.h"
#include "imgui_impl_sdl.h"

//Constructor
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Init()
{

	ImGui_ImplSdl_Init(App->window->window);

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{

	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	if (ImGui::BeginMainMenuBar()) {
		
			if (ImGui::BeginMenu("File"))
			{


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{


				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

	ImGui::Render();

	return UPDATE_CONTINUE;
	}
	


bool ModuleEditor::CleanUp()
{
	return true;
}