#include "PanelProperties.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "glew/include/glew.h"
#include "imgui_impl_sdl.h"

PanelProperties::PanelProperties(bool active = true) : Panel(active)
{}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{
	if (ImGui::BeginDock("Properties", &active))
	{
		//Transformation information

		ImGui::Text("Transformation");

		ImGui::Separator();

		//Geometry information

		ImGui::Text("Geometry");

		ImGui::Text("File name: ");
		ImGui::SameLine();
		ImGui::Text(gName);

		ImGui::Text("Number of meshes: ");
		ImGui::SameLine();
		ImGui::Text("%d", gNumMesh);

		ImGui::Text("Number of vertices: ");
		ImGui::SameLine();
		ImGui::Text("%d", gNumVert);

		ImGui::Separator();

		//Texture information

		ImGui::Text("Texture");

		ImGui::Text("Texture name: ");

		ImGui::Text("Snapshot: ");

	}

	ImGui::EndDock();
}

void PanelProperties::SaveMeshInfo(const char* file_name, int nMesh, int nVertices)
{
	gName = file_name;
	gNumMesh = nMesh;
	gNumVert = nVertices;
}
