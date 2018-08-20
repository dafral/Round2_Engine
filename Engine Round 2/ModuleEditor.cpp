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

	if (ImGui::Checkbox("Depth Test", &depth_test))
	{
		if (depth_test) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}

	if (ImGui::Checkbox("Cull Face", &cull_face))
	{
		if (cull_face) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	if (ImGui::Checkbox("Lighting", &lighting))
	{
		if (lighting) glEnable(GL_LIGHTING);
		else glDisable(GL_LIGHTING);
	}

	if (ImGui::Checkbox("Color Material", &color_material))
	{
		if (color_material) glEnable(GL_COLOR_MATERIAL);
		else glDisable(GL_COLOR_MATERIAL);
	}

	if (ImGui::Checkbox("Texture 2D", &texture_2d))
	{
		if (texture_2d) glEnable(GL_TEXTURE_2D);
		else glDisable(GL_TEXTURE_2D);
	}

	if (ImGui::Checkbox("Blend", &blend))
	{
		if (blend) glEnable(GL_BLEND);
		else glDisable(GL_BLEND);
	}

	if (ImGui::Checkbox("Dither", &dither))
	{
		if (dither) glEnable(GL_DITHER);
		else glDisable(GL_DITHER);
	}

	if (ImGui::Checkbox("Wireframe", &wireframe))
	{
		if (wireframe)
		{
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);

			glPopMatrix();
		}

		else 
		{
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);			

			glPopMatrix();
		}
	}


	ImGui::End();

	

	ImGui::Render();
}