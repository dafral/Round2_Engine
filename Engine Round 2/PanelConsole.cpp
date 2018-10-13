#include "Application.h"
#include "PanelConsole.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "glew/include/glew.h"

PanelConsole::PanelConsole(bool active = true) : Panel(active)
{

}

PanelConsole::~PanelConsole()
{

}

void PanelConsole::Draw()
{
	if (ImGui::BeginDock("Console", NULL))
	{
		ImGui::Text(App->consoleText.begin());
	}

	ImGui::EndDock();
}

void PanelConsole::ConsoleLog(const char* log)
{
	App->consoleText.appendf(log);
	App->consoleText.appendf("\n");
}