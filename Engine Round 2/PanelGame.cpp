#include "PanelGame.h"
#include "Application.h"
#include "Module.h"
#include "ModuleCamera3D.h"

#include "TextureMSAA.h"
#include "glew/include/glew.h"

PanelGame::PanelGame(bool active = true) : Panel(active)
{}

PanelGame::~PanelGame()
{}

void PanelGame::Init()
{
}

void PanelGame::Draw()
{
	if (ImGui::BeginDock("Game", NULL, ImGuiWindowFlags_NoScrollbar))
	{
		glEnable(GL_TEXTURE_2D);
		if (App->camera->GetGameTexture() != nullptr)
		{
			App->camera->GetGameTexture()->Render();
			ImGui::Image((void*)App->camera->GetGameTexture()->GetTextureID(), { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, ImVec2(0, 1), ImVec2(1, 0));
			App->camera->GetGameTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);
	}

	ImGui::EndDock();
}