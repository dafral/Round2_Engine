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

void PanelScene::Init()
{
	ImGui::SetNextWindowPos(ImVec2(0, 12));
}

void PanelScene::Draw()
{
	if (ImGui::BeginDock("Scene", NULL, ImGuiWindowFlags_NoScrollbar))
	{
		float width = GetSystemMetrics(SM_CXSCREEN) / 4;
		float height = GetSystemMetrics(SM_CYSCREEN) / 4;

		float aspect_ratio = width / height;

		//Get size of the window
		ImVec2 size = ImGui::GetContentRegionAvail();
		if (width < size.x && height < size.y)
		{
			if (width < size.x)
			{
				width = size.x;
				height = width / aspect_ratio;

				if (height > size.y)
				{
					height = size.y;
					width = height * aspect_ratio;
				}
			}

			if (height < size.y)
			{
				height = size.y;
				width = height * aspect_ratio;

				if (width > size.x)
				{
					width = size.x;
					height = width / aspect_ratio;
				}
			}
		}
	
		glEnable(GL_TEXTURE_2D);
		if (App->camera->GetSceneTexture() != nullptr)
		{
			App->camera->GetSceneTexture()->Render();
			ImGui::Image((void*)App->camera->GetSceneTexture()->GetTextureID(), { width, height }, ImVec2(0, 1), ImVec2(1, 0));
			App->camera->GetSceneTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);
	}

	ImGui::EndDock();
}