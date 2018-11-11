#include "PanelScene.h"
#include "Application.h"
#include "Module.h"
#include "ModuleCamera3D.h"
#include "DebugDraw.h"
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
		bool debug_draw = App->debug->IsDebugDrawActive();

		if (ImGui::Checkbox("Debug Draw", &debug_draw))
			App->debug->SetDebugDraw(debug_draw);

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