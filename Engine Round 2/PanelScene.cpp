#include "PanelScene.h"
#include "Application.h"
#include "Module.h"
#include "ModuleCamera3D.h"

#include "ImGui/imgui.h"
#include "glew/include/glew.h"
#include "ImGui/imgui_dock.h"

PanelScene::PanelScene(bool active = true) : Panel(active)
{}

PanelScene::~PanelScene()
{}

void PanelScene::Draw()
{
	if (ImGui::BeginDock("Scene", &active))
	{
		//Get size of the window
		ImVec2 size = ImGui::GetContentRegionAvail();

		//Render the texture
		glEnable(GL_TEXTURE_2D);

		if (App->camera->GetSceneTexture() != nullptr)
		{
			ImGui::Image((void*)App->camera->GetSceneTexture()->GetTextureID(), size, ImVec2(0, 1), ImVec2(1, 0));
		}

		glDisable(GL_TEXTURE_2D);
	}

	ImGui::EndDock();

	if (App->camera->GetSceneTexture() != nullptr)
	{
		App->camera->GetSceneTexture()->Render();
		App->camera->GetSceneTexture()->Unbind();
	}
}