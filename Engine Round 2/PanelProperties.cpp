#include "Application.h"
#include "ModuleEditor.h"
#include "PanelInspector.h"
#include "PanelProperties.h"
#include "GameObject.h"
#include "Component_Transform.h"
#include "Component_Mesh.h"
#include "Component_Material.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"

PanelProperties::PanelProperties(bool active = true) : Panel(active)
{}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Init() 
{
	ImGui::SetNextDock("Properties", ImGuiDockSlot_Left);
}

void PanelProperties::Draw()
{
	if (ImGui::BeginDock("Properties", NULL))
	{
		GameObject* go = App->editor->inspector->GetSelectedGO();

		if (active && go != nullptr)
		{
			Component_Transform* ctrans = (Component_Transform*)go->FindComponentWithType(TRANSFORM);
			Component_Mesh* cmesh = (Component_Mesh*)go->FindComponentWithType(MESH);
			Component_Material* cmaterial = (Component_Material*)go->FindComponentWithType(MATERIAL);

			GOInfo(go);

			ImGui::Separator();

			TransInfo(ctrans, go->GetStatic());

			ImGui::Separator();

			MeshInfo(cmesh);

			ImGui::Separator();

			MaterialInfo(cmaterial);
		}
	}

	ImGui::EndDock();
}

void PanelProperties::GOInfo(GameObject* go)
{
	bool is_static = go->GetStatic();
	bool is_visible = go->GetVisible();

	ImGui::Text("Name: %s", go->name.c_str());

	ImGui::Separator();

	if (ImGui::Checkbox("Static", &is_static))
		go->SetStatic(is_static);

	ImGui::SameLine();

	if (ImGui::Checkbox("Visible", &is_visible))
		go->SetVisible(is_visible);
}

void PanelProperties::MeshInfo(Component_Mesh* mesh)
{
	if (mesh != nullptr)
	{
		if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Vertices: %d", mesh->GetNumVertices());
			ImGui::Text("Triangles: %d", mesh->GetNumIndices() / 3);
		}
	}
}

void PanelProperties::MaterialInfo(Component_Material* material)
{
	if (material != nullptr)
	{
		if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Dimensions: %d x %d", material->GetTextureWidth(), material->GetTextureHeight());
			ImGui::Image(material->GetTextureSnap(), ImVec2(150.0f, 150.0f));
		}
	}
}

void PanelProperties::TransInfo(Component_Transform* trans, bool is_static)
{
	if (trans != nullptr)
	{
		float3 position = trans->GetPosition();
		float3 scale = trans->GetScale();
		Quat rotation = trans->GetRotation();

		float3 euler_rotation = RadToDeg(rotation.ToEulerXYZ());

		ImGui::Text("    X         Y        Z");

		if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
			if (!is_static) trans->SetPosition(position);

		if (ImGui::DragFloat3("Rotation", (float*)&euler_rotation, 0.1f))
			if (!is_static) trans->SetRotation(DegToRad(euler_rotation));

		if (ImGui::DragFloat3("Scale", (float*)&scale, 1.0f))
			if (!is_static) trans->SetScale(scale);
	}
}
