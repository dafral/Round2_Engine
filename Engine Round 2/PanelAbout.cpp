#include "PanelAbout.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "glew/include/glew.h"
#include "imgui_impl_sdl.h"

PanelAbout::PanelAbout(bool active = true) : Panel(active)
{

}

PanelAbout::~PanelAbout()
{

}

void PanelAbout::Draw()
{
	if (ImGui::BeginDock("About", &active))
	{
		ImGui::Text("Glew version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GLEW_VERSION));

		ImGui::Text("Vendor: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_VENDOR));

		ImGui::Text("Renderer: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_RENDERER));

		ImGui::Text("OpenGL version supported: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_VERSION));

		ImGui::Text("GLSL: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	
	ImGui::EndDock();
}