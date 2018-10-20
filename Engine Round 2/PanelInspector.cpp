#include "PanelInspector.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "glew/include/glew.h"

PanelInspector::PanelInspector(bool active = true) : Panel(active)
{

}

PanelInspector::~PanelInspector()
{

}

void PanelInspector::Init()
{
	ImGui::SetNextDock("Scene", ImGuiDockSlot_Left);
}

void PanelInspector::Draw()
{
	if (ImGui::BeginDock("Inspector", NULL))
	{

	}

	ImGui::EndDock();
}