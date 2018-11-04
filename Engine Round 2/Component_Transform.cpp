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

void Component_Transform::OnSave(JSON_Doc& config)
{

}

void Component_Transform::OnLoad(JSON_Doc * config)
{

}
