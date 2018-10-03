#include "PanelConsole.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "glew/include/glew.h"
#include "imgui_impl_sdl.h"

PanelConsole::PanelConsole(bool active = true) : Panel(active)
{

}

PanelConsole::~PanelConsole()
{

}

void PanelConsole::Draw()
{
	if (ImGui::BeginDock("Console", &active))
	{
		ImGui::Text("-----------------------------------------------");

	}

	ImGui::EndDock();
}