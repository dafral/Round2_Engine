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
	
	}
	
	ImGui::EndDock();
}

