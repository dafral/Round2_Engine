#include "PanelScene.h"
#include "Application.h"
#include "Module.h"
#include "ModuleCamera3D.h"
#include "TextureMSAA.h"
#include "glew/include/glew.h"

PanelScene::PanelScene(bool active = true) : Panel(active)
{}

PanelScene::~PanelScene()
{}

void PanelScene::Init()
{
	ImGui::SetNextWindowPos(ImVec2(0, 12));
}

void PanelScene::Draw()
{
	if (ImGui::BeginDock("Scene", NULL, ImGuiWindowFlags_NoScrollbar))
	{
		glEnable(GL_TEXTURE_2D);
		if (App->camera->GetSceneTexture() != nullptr)
		{
			App->camera->GetSceneTexture()->Render();
			ImGui::Image((void*)App->camera->GetSceneTexture()->GetTextureID(), { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, ImVec2(0, 1), ImVec2(1, 0));
			App->camera->GetSceneTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);
	}

	ImGui::EndDock();
}

void PanelScene::Update()
{
	SetHovered(ImGui::IsWindowHovered());
}

float2 PanelScene::GetMousePosNormalized()
{
	ImVec2 mouse_pos, mouse_dock_pos, window_pos;

	mouse_pos = ImGui::GetMousePos();
	window_pos = ImGui::GetWindowPos();

	mouse_dock_pos.x = mouse_pos.x - window_pos.x;
	mouse_dock_pos.y = mouse_pos.y - window_pos.y;

	CONSOLELOG("x:%f, y:%f", mouse_dock_pos.x, mouse_dock_pos.y);

	float2 normalized_pos = { mouse_dock_pos.x, mouse_dock_pos.y };
	normalized_pos = normalized_pos.Normalized();

	return normalized_pos;
}