#include "PanelProperties.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "glew/include/glew.h"
#include "imgui_impl_sdl.h"

PanelProperties::PanelProperties(bool active = true) : Panel(active)
{}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{
	if (ImGui::BeginDock("Properties", &active))
	{
		ImGui::Text("Transformation: ");

		ImGui::Text("Geometry: ");

		ImGui::Text("Texture: ");

	}

	ImGui::EndDock();
}
