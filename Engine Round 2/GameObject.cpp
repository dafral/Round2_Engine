#include "GameObject.h"

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{}

GameObject::~GameObject()
{}

void GameObject::AddChildren(GameObject* children)
{
	if (children != nullptr)
		childrens.push_back(children);
}

void GameObject::AddComponent(Component* component)
{
	if (component != nullptr)
		components.push_back(component);
}

Component* GameObject::FindComponentWithType(Component_Type type)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type) return components[i];
	}

	return nullptr;
}

void GameObject::SetStatic(bool new_static) 
{
	is_static = new_static; 

	// Recursion
	for (int i = 0; i < childrens.size(); i++)
	{
		childrens[i]->SetStatic(new_static);
	}
};

void GameObject::SetVisible(bool new_visible)
{ 
	is_visible = new_visible; 

	// Recursion
	for (int i = 0; i < childrens.size(); i++)
	{
		childrens[i]->SetVisible(new_visible);
	}
};

// ------------------------------------------------------------------

void GameObject::OnSave(JSON_Doc& config)
{
	Component_Transform* trans = (Component_Transform*)FindComponentWithType(TRANSFORM);
	Component_Mesh* mesh = (Component_Mesh*)FindComponentWithType(MESH);
	Component_Material* material = (Component_Material*)FindComponentWithType(MATERIAL);
	if (trans)
		trans->OnSave(*App->json->config);
	if (mesh)
		mesh->OnSave(*App->json->config);
	if (material)
		material->OnSave(*App->json->config);
}
void GameObject::OnLoad(JSON_Doc * config)
{
}