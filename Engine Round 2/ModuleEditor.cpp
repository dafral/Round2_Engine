#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ImGui\imgui.h"
#include "imgui_impl_sdl.h"

#include "glew/include/glew.h"

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
	

	return UPDATE_CONTINUE;
	}
	


bool ModuleEditor::CleanUp()
{
	return true;
}

void ModuleEditor::Draw()
{
	////Prueba
	ImGui::Begin("About", NULL, ImGuiWindowFlags_NoScrollbar);

	ImGui::Text("Glew version: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GLEW_VERSION));

	ImGui::Text("Vendor: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GL_VENDOR));

	ImGui::Text("Renderer: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GL_RENDERER));

	ImGui::Text("OpenGL version supported: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GL_VERSION));

	ImGui::Text("GLSL: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GL_SHADING_LANGUAGE_VERSION));

	ImGui::End();

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
}