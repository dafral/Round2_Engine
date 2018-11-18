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
{}

void PanelScene::Draw()
{
	if (ImGui::BeginDock("Scene", NULL, ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 new_size = ImGui::GetContentRegionAvail();
		if (region.x != new_size.x || region.y != new_size.y)
		{
			region.x = new_size.x;
			region.y = new_size.y;
		}

		SetHovered(ImGui::IsWindowHovered());
		window_pos = { ImGui::GetWindowPos().x + ImGui::GetCursorPosX(), ImGui::GetWindowPos().y + ImGui::GetCursorPosY() };

		ImVec2 size = ImGui::GetContentRegionAvail();

		glEnable(GL_TEXTURE_2D);
		if (App->camera->GetSceneTexture() != nullptr)
		{
			App->camera->GetSceneTexture()->Render();
			ImGui::Image((void*)App->camera->GetSceneTexture()->GetTextureID(), size, ImVec2(0, 1), ImVec2(1, 0));
			App->camera->GetSceneTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);
	}

	ImGui::EndDock();
}

float2 PanelScene::GetMousePosNormalized()
{
	ImVec2 mouse_pos = { (((App->input->GetMouseX() - window_pos.x) / region.x)),     // 1 = margin
						 (((App->input->GetMouseY() - window_pos.y) / region.y)) };

	mouse_pos.x -= 0.5;
	mouse_pos.x *= 2;

	mouse_pos.y -= 0.5;
	mouse_pos.y *= -2;

	CONSOLELOG("x:%f, y:%f", mouse_pos.x, mouse_pos.y);
	
	return { mouse_pos.x, mouse_pos.y };
}