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
		float width = GetSystemMetrics(SM_CXSCREEN) / 3;
		float height = GetSystemMetrics(SM_CYSCREEN) / 3;

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
		if (App->camera->GetGameTexture() != nullptr)
		{
			App->camera->GetGameTexture()->Render();
			ImGui::Image((void*)App->camera->GetGameTexture()->GetTextureID(), { width, height }, ImVec2(0, 1), ImVec2(1, 0));
			App->camera->GetGameTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);
	}

	ImGui::EndDock();
}