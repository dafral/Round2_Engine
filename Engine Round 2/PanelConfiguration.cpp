#include "PanelConfiguration.h"
#include "ImGui/imgui.h"
#include "glew/include/glew.h"
#include "ImGui/imgui_dock.h"

PanelConfiguration::PanelConfiguration(bool active = true) : Panel(active)
{

}

PanelConfiguration::~PanelConfiguration()
{

}

void PanelConfiguration::Draw()
{
	if (ImGui::BeginDock("Configuration", &active))
	{
		if (ImGui::Checkbox("Depth Test", &depth_test))
		{
			if (depth_test) glEnable(GL_DEPTH_TEST);
			else glDisable(GL_DEPTH_TEST);
		}

		if (ImGui::Checkbox("Cull Face", &cull_face))
		{
			if (cull_face) glEnable(GL_CULL_FACE);
			else glDisable(GL_CULL_FACE);
		}

		if (ImGui::Checkbox("Lighting", &lighting))
		{
			if (lighting) glEnable(GL_LIGHTING);
			else glDisable(GL_LIGHTING);
		}

		if (ImGui::Checkbox("Color Material", &color_material))
		{
			if (color_material) glEnable(GL_COLOR_MATERIAL);
			else glDisable(GL_COLOR_MATERIAL);
		}

		if (ImGui::Checkbox("Texture 2D", &texture_2d))
		{
			if (texture_2d) glEnable(GL_TEXTURE_2D);
			else glDisable(GL_TEXTURE_2D);
		}

		if (ImGui::Checkbox("Blend", &blend))
		{
			if (blend) glEnable(GL_BLEND);
			else glDisable(GL_BLEND);
		}

		if (ImGui::Checkbox("Dither", &dither))
		{
			if (dither) glEnable(GL_DITHER);
			else glDisable(GL_DITHER);
		}

		if (ImGui::Checkbox("Wireframe", &wireframe))
		{
			if (wireframe)
			{
				glPolygonMode(GL_FRONT, GL_LINE);
				glPolygonMode(GL_BACK, GL_LINE);
				glPopMatrix();
			}
			else
			{
				glPolygonMode(GL_FRONT, GL_FILL);
				glPolygonMode(GL_BACK, GL_FILL);
				glPopMatrix();
			}
		}
	}

	ImGui::EndDock();
}