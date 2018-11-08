#include "GameObject.h"
#include "Application.h"
#include "Component_Transform.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Camera.h"

#include "PCG/pcg_basic.h"

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{
	unique_id = pcg32_random_r(&App->rng);
}

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

void GameObject::OnSave(JSON_Doc* config)
{

	std::string aux = std::string("Gameobjects.") + name + std::string(".Name");
	config->SetString(aux.c_str(), name.c_str());

	aux = std::string("Gameobjects.") + name + std::string(".Visible");
	config->SetBool(aux.c_str(), is_visible);

	aux = std::string("Gameobjects.") + name + std::string(".Static");
	config->SetBool(aux.c_str(), is_static);

	if (parent != nullptr) {

		aux = std::string("Gameobjects.") + name + std::string(".Parent");
		config->SetNumber(aux.c_str(), parent->unique_id);
	}
	else {
		aux = std::string("Gameobjects.") + name + std::string(".Parent");
		config->SetNumber(aux.c_str(), 0);

	}

	//Component Transform
	Component_Transform* trans = (Component_Transform*)FindComponentWithType(TRANSFORM);

	//Position
	aux = std::string("Gameobjects.") + name + std::string(".Pos.x");
	config->SetNumber(aux.c_str(), trans->GetPosition().x);

	aux = std::string("Gameobjects.") + name + std::string(".Pos.y");
	config->SetNumber(aux.c_str(), trans->GetPosition().y);

	aux = std::string("Gameobjects.") + name + std::string(".Pos.z");
	config->SetNumber(aux.c_str(), trans->GetPosition().z);

	//Rotation
	aux = std::string("Gameobjects.") + name + std::string(".Rot.x");
	config->SetNumber(aux.c_str(), trans->GetRotation().x);

	aux = std::string("Gameobjects.") + name + std::string(".Rot.y");
	config->SetNumber(aux.c_str(), trans->GetRotation().y);

	aux = std::string("Gameobjects.") + name + std::string(".Rot.z");
	config->SetNumber(aux.c_str(), trans->GetRotation().z);

	aux = std::string("Gameobjects.") + name + std::string(".Rot.w");
	config->SetNumber(aux.c_str(), trans->GetRotation().w);

	//Scale
	aux = std::string("Gameobjects.") + name + std::string(".Scale.x");
	config->SetNumber(aux.c_str(), trans->GetScale().x);

	aux = std::string("Gameobjects.") + name + std::string(".Scale.y");
	config->SetNumber(aux.c_str(), trans->GetScale().y);

	aux = std::string("Gameobjects.") + name + std::string(".Scale.z");
	config->SetNumber(aux.c_str(), trans->GetScale().z);


	//Travel Childs
	for (int i = 0; i < childrens.size(); i++) {
		childrens[i]->OnSave(config);
	}


};

void GameObject::OnLoad(JSON_Doc* config)
{};

