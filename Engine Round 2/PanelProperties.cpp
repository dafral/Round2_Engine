#include "Application.h"
#include "PanelHierarchy.h"
#include "PanelProperties.h"

#include "Component_Transform.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Camera.h"
#include "GameObject.h"

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
		GameObject* go = App->editor->hierarchy->GetSelectedGO();
		
		if (isActive() && go != nullptr)
		{
			Component_Transform* ctrans = (Component_Transform*)go->FindComponentWithType(TRANSFORM);
			Component_Mesh* cmesh = (Component_Mesh*)go->FindComponentWithType(MESH);
			Component_Material* cmaterial = (Component_Material*)go->FindComponentWithType(MATERIAL);
			Component_Camera* ccamera = (Component_Camera*)go->FindComponentWithType(CAMERA);

			GOInfo(go);
			TransInfo(ctrans, ccamera, go->GetStatic());
			MeshInfo(cmesh);
			MaterialInfo(cmaterial);
			CameraInfo(ccamera);
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
	{
		App->octree->CleanUp();
		go->SetStatic(is_static);
		App->octree->StartOctree();
	}

	ImGui::SameLine();

	if (ImGui::Checkbox("Visible", &is_visible))
		go->SetVisible(is_visible);
}

void PanelProperties::MeshInfo(Component_Mesh* mesh)
{
	if (mesh != nullptr)
	{
		ImGui::Separator();

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
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Dimensions: %d x %d", material->GetTextureWidth(), material->GetTextureHeight());
			ImGui::Image(material->GetTextureSnap(), ImVec2(150.0f, 150.0f));
		}
	}
}

void PanelProperties::TransInfo(Component_Transform* trans, Component_Camera* cam, bool is_static)
{
	if (trans != nullptr)
	{
		ImGui::Separator();

		float3 position = trans->GetPosition();
		float3 scale = trans->GetScale();
		Quat rotation = trans->GetRotation();

		float3 euler_rotation = RadToDeg(rotation.ToEulerXYZ());

		if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
		{
			if (!is_static) trans->SetPosition(position);
			if (cam != nullptr) cam->SetFrustumPos(trans->GetPosition());
		}
		if (ImGui::DragFloat3("Rotation", (float*)&euler_rotation, 0.1f))
		{
			if (!is_static) trans->SetRotation(DegToRad(euler_rotation));
			if (cam != nullptr) cam->SetFrustumRot(trans->GetRotation());
		}
		if (ImGui::DragFloat3("Scale", (float*)&scale, 1.0f))
			if (!is_static) trans->SetScale(scale);
	}
}

void PanelProperties::CameraInfo(Component_Camera* cam)
{
	if (cam != nullptr)
	{
		ImGui::Separator();

		int v_fov = cam->GetFOV();

		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::SliderInt("Field of View", &v_fov, 50, 100))
				cam->SetFOV(v_fov);
		}
	}
}
