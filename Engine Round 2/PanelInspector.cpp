#include "Application.h"
#include "ModuleScene.h"
#include "PanelInspector.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"

PanelInspector::PanelInspector(bool active = true) : Panel(active)
{}

PanelInspector::~PanelInspector()
{}

void PanelInspector::Init()
{
	ImGui::SetNextDock("Scene", ImGuiDockSlot_Left);
}

void PanelInspector::Draw()
{
	if (ImGui::BeginDock("Inspector", NULL))
	{
		CreateHierarchy(App->scene->root_node);
	}

	ImGui::EndDock();
}

void PanelInspector::CreateHierarchy(GameObject* curr_go)
{
	uint flag = 0; // Deafult

	// Setting flags
	if (curr_go->childrens.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (go_selected == curr_go)
		flag |= ImGuiTreeNodeFlags_Selected;

	if (curr_go == App->scene->root_node)
	{
		// Recursion
		for (int i = 0; i < curr_go->childrens.size(); i++)
			CreateHierarchy(curr_go->childrens[i]);
	}
	else
	{
		// Creating tree
		if (ImGui::TreeNodeEx(curr_go->name.c_str(), flag))
		{
			// Know if a GO is selected
			if (ImGui::IsItemClicked())
				go_selected = curr_go;

			if (ImGui::IsMouseDoubleClicked(1) && !ImGui::IsWindowFocused())
				go_selected = nullptr;

			// Recursion
			for (int i = 0; i < curr_go->childrens.size(); i++)
				CreateHierarchy(curr_go->childrens[i]);

			ImGui::TreePop();
		}
	}	
}