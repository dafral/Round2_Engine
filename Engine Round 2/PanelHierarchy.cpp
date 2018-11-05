#include "Application.h"
#include "ModuleScene.h"
#include "PanelHierarchy.h"
#include "Component_Camera.h"

PanelHierarchy::PanelHierarchy(bool active = true) : Panel(active)
{}

PanelHierarchy::~PanelHierarchy()
{}

void PanelHierarchy::Init()
{
	ImGui::SetNextDock("Scene", ImGuiDockSlot_Left);
}

void PanelHierarchy::Draw()
{
	if (ImGui::BeginDock("Hierarchy", NULL))
	{
		CreateHierarchy(App->scene->root_node);
	}

	ImGui::EndDock();
}

void PanelHierarchy::CreateHierarchy(GameObject* curr_go)
{
	uint flag = 0; // Deafult

	// Setting flags
	if (curr_go->childrens.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (go_selected == curr_go)
		flag |= ImGuiTreeNodeFlags_Selected;

	if (curr_go == App->scene->root_node || curr_go == App->camera->GetSceneCamera()->my_go)
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