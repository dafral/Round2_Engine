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

void PanelConfiguration::Init()
{
	ImGui::SetNextDock("Scene", ImGuiDockSlot_Right);
}

void PanelConfiguration::Draw()
{
	if (ImGui::BeginDock("Configuration", NULL))
	{
		ApplicationConfig();
		ImGui::Separator();

		RendererConfig();
		ImGui::Separator();

		WindowConfig();
		ImGui::Separator();

		InputConfig();
		ImGui::Separator();

		TexturesConfig();
		ImGui::Separator();

		AboutConfig();
	}

	ImGui::EndDock();
}

void PanelConfiguration::AddFps(float fps)
{
	if (fps_buffer.size() > 50)
		fps_buffer.erase(fps_buffer.begin());

	fps_buffer.push_back(fps);
}

void PanelConfiguration::AddMs(float ms)
{
	if (ms_buffer.size() > 50)
		ms_buffer.erase(ms_buffer.begin());

	ms_buffer.push_back(ms);
}

int PanelConfiguration::GetFPSCap()
{
	if (fps_cap == 0) return 60;
	else return fps_cap;
}

void PanelConfiguration::ApplicationConfig()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		char title[25];

		ImGui::SliderInt(" Max FPS", &fps_cap, 0, 100);

		if(ImGui::Checkbox("Vsync", &vsync)) App->renderer3D->SetVsync(vsync);

		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%i", fps_cap);

		//fps
		sprintf_s(title, 25, "Framerate %.1f", fps_buffer[fps_buffer.size() - 1]);
		ImGui::PlotHistogram("##Framerate", &fps_buffer[0], fps_buffer.size(), 0, title, 0.0f, 100.0f, ImVec2(300, 120));

		// dt
		sprintf_s(title, 25, "Miliseconds %.1f", ms_buffer[ms_buffer.size() - 1]);
		ImGui::PlotHistogram("##Dt", &ms_buffer[0], ms_buffer.size(), 0, title, 0.0f, 100.0f, ImVec2(300, 120));
	}
}

void PanelConfiguration::RendererConfig()
{
	if (ImGui::CollapsingHeader("Renderer"))
	{
		ImGui::Checkbox("Grid", &grid);
		ImGui::Separator();

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

		//if (ImGui::Checkbox("Color Material", &color_material))
		//{
		//	if (color_material) glEnable(GL_COLOR_MATERIAL);
		//	else glDisable(GL_COLOR_MATERIAL);
		//}

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

		ImGui::Checkbox("Wireframe Mode", &wireframe);
		ImGui::Checkbox("Points Mode", &points);

		if (ImGui::Checkbox("Color", &color))
		{
			if (color) glEnable(GL_COLOR);
			else glDisable(GL_COLOR);
		}

		ImGui::ColorPicker3("Geometry color", App->renderer3D->color_hue);
	}
}

void PanelConfiguration::WindowConfig()
{
	if (ImGui::CollapsingHeader("Window")) 
	{
		float brightness =  App->window->GetBrightness();
		if (ImGui::SliderFloat("Brightness", &brightness, 0, 1))
			App->window->SetBrightness(brightness);

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
		if (ImGui::Button("Delete Texture"))
		{
			App->material->DeleteTextures();
		}
		
	}
}

void PanelConfiguration::AboutConfig()
{
	if (ImGui::CollapsingHeader("About"))
	{
		//Software
		ImGui::TextColored(ImVec4(1.00f, 1.00f, 0.00f, 1.00f), "Software");
		ImGui::Separator();

		ImGui::Text("Glew version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GLEW_VERSION));

		ImGui::Text("GLSL: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_SHADING_LANGUAGE_VERSION));

		ImGui::Text("SDL version: ");
		ImGui::SameLine();
		SDL_version version;
		SDL_VERSION(&version);
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%d.%d.%d", version.major, version.minor, version.patch);

		ImGui::Text("OpenGL version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glewGetString(GL_VERSION));

		ImGui::Text("Assimp version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "3.1.1");

		ImGui::Text("DevIL version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "1.8.0");

		ImGui::Text("MathGeoLib version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "1.5");

		ImGui::Text("ImguiDock: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "1.0");

		ImGui::Separator();

		//Hardware
		ImGui::TextColored(ImVec4(1.00f, 1.00f, 0.00f, 1.00f), "Hardware");
		ImGui::Separator();

		//CPU 
		ImGui::Text("Number of logical CPU cores : ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

		//RAM
		ImGui::Text("Number of system RAM: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%dGb", SDL_GetCPUCount());

		//CAPS
		ImGui::Text("Caps: ");
		ImGui::SameLine();
		if (SDL_Has3DNow)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "3DNow");
		ImGui::SameLine();
		if (SDL_HasAVX)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "AVX");
		ImGui::SameLine();
		if (SDL_HasAltiVec)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "AltiVec");
		ImGui::SameLine();
		if (SDL_HasMMX)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "MMX");
		ImGui::SameLine();
		if (SDL_HasRDTSC)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "RDTSC");

		if (SDL_HasSSE)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "\t\tSSE");
		ImGui::SameLine();
		if (SDL_HasSSE2)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "SSE2");
		ImGui::SameLine();
		if (SDL_HasSSE3)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "SSE3");
		ImGui::SameLine();
		if (SDL_HasSSE41)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "SSE41");
		ImGui::SameLine();
		if (SDL_HasSSE42)ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "SSE42");

		//GPU
		ImGui::Text("Vendor: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_RENDERER));
		ImGui::Text("Version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.00f, 0.40f, 0.00f, 1.00f), "%s", glGetString(GL_VERSION));
	}
}