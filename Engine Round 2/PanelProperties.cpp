#include "PanelProperties.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"
#include "glew/include/glew.h"

PanelProperties::PanelProperties(bool active = true) : Panel(active)
{}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{
	if (ImGui::BeginDock("Properties", NULL))
	{
		//Transformation information

		ImGui::TextColored(ImVec4(1.00f, 1.00f, 0.00f, 1.00f), "Transformation");

		ImGui::Text("Position: X %f  Y %f  Z %f", position.x, position.y, position.z);
		ImGui::Text("Rotation: X %f  Y %f  Z %f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("Scale: X %f  Y %f  Z %f", scale.x, scale.y, scale.z);

		ImGui::Separator();

		//Geometry information

		ImGui::TextColored(ImVec4(1.00f, 1.00f, 0.00f, 1.00f), "Geometry");

		ImGui::Text("File path: ");
		ImGui::SameLine();
		ImGui::Text(gName);

		ImGui::Text("Number of meshes: ");
		ImGui::SameLine();
		ImGui::Text("%d", gNumMesh);

		ImGui::Text("Number of vertices: ");
		ImGui::SameLine();
		ImGui::Text("%d", gNumVert);

		ImGui::Text("Number of triangles: ");
		ImGui::SameLine();
		ImGui::Text("%d", gNumTriangles);

		ImGui::Separator();

		//Texture information

		ImGui::TextColored(ImVec4(1.00f, 1.00f, 0.00f, 1.00f), "Texture");

		ImGui::Text("Texture path: ");
		ImGui::SameLine();
		ImGui::Text(tName);

		ImGui::Text("Texture format: ");
		ImGui::SameLine();
		ImGui::Text(tType.c_str());

		ImGui::Text("Texture Dimensions: ");
		ImGui::SameLine();
		ImGui::Text("%d x %d", tWidth, tHeight);

		ImGui::Text("Texture Snapshot: ");
		if (tSnap != nullptr) 
		{
			ImGui::Image(tSnap, ImVec2(200.0f, 200.0f));
		}

	}

	ImGui::EndDock();
}

void PanelProperties::SaveTransformationInfo(aiVector3D pos, aiQuaternion rot, aiVector3D scl)
{
	position = (pos.x, pos.y, pos.z);
	rotation = (rot.GetEuler().x * RADTODEG , rot.GetEuler().y * RADTODEG, rot.GetEuler().z * RADTODEG);
	scale = (scl.x, scl.y, scl.z);
}

void PanelProperties::SaveMeshInfo(const char* file_name, int nMesh, int nVertices, int nTriangles)
{
	gName = file_name;
	gNumMesh = nMesh;
	gNumVert = nVertices;
	gNumTriangles = nTriangles;
}

void PanelProperties::EraseGeometryInfo()
{
	gName = " ";
	gNumMesh = 0;
	gNumVert = 0;
	gNumTriangles = 0;
}

void PanelProperties::EraseTextureInfo()
{
	tName = " ";
	tType = " ";
	tWidth = 0;
	tHeight = 0;
	tSnap = nullptr;
}

void PanelProperties::SaveTextureInfo(const char* file_name, int width, int height, ImTextureID snapshot)
{
	tName = file_name;

	std::string str = file_name;
	int dot_pos = str.find_last_of(".");
	tType = str.substr(dot_pos + 1, 3);
	

	tWidth = width;
	tHeight = height;

	tSnap = snapshot;
}
