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
		SetHovered(ImGui::IsWindowHovered());
		window_pos = { ImGui::GetWindowPos().x + ImGui::GetCursorPos().x, ImGui::GetWindowPos().y + ImGui::GetCursorPos().y };
		region = ImGui::GetContentRegionAvail();

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

float2 PanelScene::GetMousePosNormalized()
{
	ImVec2 mouse_pos = { ImGui::GetMousePos().x - window_pos.x - 1,  // 1 = margin
		                 ImGui::GetMousePos().y - window_pos.y - 1};

	ImVec2 mouse_normalized;

	mouse_normalized.x = mouse_pos.x / region.x;
	mouse_normalized.y = mouse_pos.y / region.y;

	mouse_normalized.x -= 0.5;
	mouse_normalized.x *= 2;

	mouse_normalized.y -= 0.5;
	mouse_normalized.y *= 2;

	mouse_normalized.y *= -1;

	CONSOLELOG("x:%f, y:%f", mouse_pos.x, mouse_pos.y);
	
	return { mouse_normalized.x, mouse_normalized.y };
}