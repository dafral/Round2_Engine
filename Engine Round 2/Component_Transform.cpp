#include "Component_Transform.h"
#include "GameObject.h"

Component_Transform::Component_Transform() : Component(TRANSFORM)
{
	position = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
};

Component_Transform::~Component_Transform() 
{};

float4x4 Component_Transform::GetTransform()
{
	transform = transform.FromTRS(position, rotation, scale);
	return transform;
};

const float4x4 Component_Transform::GetGlobalTransform()
{
	float4x4 ret = GetTransform().Transposed();

	if (my_go->parent != nullptr)
	{
		Component_Transform* parent_trans = (Component_Transform*)my_go->parent->FindComponentWithType(TRANSFORM);

		if (parent_trans != nullptr) 
			ret = ret * parent_trans->GetTransform().Transposed();
	}

	return ret;
}

// -----------------------------------------------------------------

void Component_Transform::OnSave(JSON_Doc* filetosave)
{
	filetosave->AddSectionToArray("Components");
	filetosave->MoveToSectionFromArray("Components", filetosave->GetArraySize("Components") - 1);

	filetosave->SetNumber("type", 0);
	filetosave->SetNumber("owner", my_go->GetUniqueID());
	filetosave->SetNumber3("position", position);
	filetosave->SetNumber3("scale", scale);

	float4 rot(rotation.x, rotation.y, rotation.z, rotation.w);
	filetosave->SetNumber4("rotation", rot);
}