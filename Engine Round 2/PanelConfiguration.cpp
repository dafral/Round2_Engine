#include "Application.h"
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
		RendererConfig();
		ImGui::Separator();

		WindowConfig();
		ImGui::Separator();

		InputConfig();
		ImGui::Separator();

		TexturesConfig();
	}

	ImGui::EndDock();
}

void PanelConfiguration::RendererConfig()
{
	if (ImGui::CollapsingHeader("Renderer")) {

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
			if (texture_2d)
				glEnable(GL_TEXTURE_2D);
			else
				glDisable(GL_TEXTURE_2D);
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
}

void PanelConfiguration::WindowConfig()
{
	if (ImGui::CollapsingHeader("Window")) 
	{
		float brightness =  App->window->GetBrightness();
		if (ImGui::SliderFloat("Brightness", &brightness, 0, 1))
			App->window->SetBrightness(brightness);

		int width = App->window->GetScreenWidth();
		if (ImGui::SliderInt("Width", &width, 900, 1920))
		{
			App->window->SetScreenWidth(width);
			App->renderer3D->OnResize(App->window->GetScreenWidth(), App->window->GetScreenHeight());
		}

		int height = App->window->GetScreenHeight();
		if (ImGui::SliderInt("Height", &height, 600, 1080))
		{
			App->window->SetScreenHeight(height);
			App->renderer3D->OnResize(App->window->GetScreenWidth(), App->window->GetScreenHeight());
		}

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->SwitchFullscreen();

		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->SwitchResizable();

		if (ImGui::Checkbox("Borderless", &bordered))
			App->window->SwitchBordered();
	}
}

void PanelConfiguration::InputConfig()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse coords:\nX: %d\tY: %d", App->input->GetMouseX(), App->input->GetMouseY());

		ImGui::Separator();

		ImGui::Text("Camera configuration");
		ImGui::Checkbox("Invert X axis", &App->camera->orb_x_inverted);
		ImGui::Checkbox("Invert Y axis", &App->camera->orb_y_inverted);
		ImGui::Checkbox("Invert zoom", &App->camera->wheel_inverted);

		sensitivity = App->camera->GetSensitivity();

		if (ImGui::SliderFloat("Camera sensitivity", &sensitivity, 0.10f, 1.00f))
			App->camera->SetSensitivity(sensitivity);
	}
}

void PanelConfiguration::TexturesConfig()
{
	if (ImGui::CollapsingHeader("Textures"))
	{

	}
}