#include "Application.h"
#include "PanelTime.h"
#include "ModuleScene.h"

PanelTime::PanelTime(bool active = true) : Panel(active)
{}

PanelTime::~PanelTime()
{}

void PanelTime::Init()
{}

void PanelTime::Draw()
{
	if (ImGui::Begin("Time Manager", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		if (ImGui::Button("Play"))
		{
			App->scene->Play();
		}
		ImGui::SameLine();

		if (ImGui::Button("Pause"))
		{
			App->scene->Pause();
		}
		ImGui::SameLine();

		if (ImGui::Button("Tick"))
		{
			App->scene->Tick();
		}
		ImGui::SameLine();

		ImGui::Text("%.2f seconds", App->scene->ReadTimer());

		ImGui::End();
	}

	bool debug_draw = App->debug->IsDebugDrawActive();

	if (ImGui::Begin("Debug", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		if (ImGui::Checkbox("Debug Draw", &debug_draw))
			App->debug->SetDebugDraw(debug_draw);

		ImGui::End();
	}
}

