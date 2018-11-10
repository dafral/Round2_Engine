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

void GameObject::OnSave(JSON_Doc* filetosave)
{

	//Save this game object
	filetosave->AddSectionToArray("GameObjects");
	filetosave->MoveToSectionFromArray("GameObjects", filetosave->GetArraySize("GameObjects") - 1);
	filetosave->SetNumber("unique ID", unique_id);
	filetosave->SetString("name", name.c_str());
	filetosave->SetBool("is_static", is_static);
	if (parent != nullptr)
		filetosave->SetNumber("parent", parent->GetUniqueID());
	else
		filetosave->SetNumber("parent", 0);
	filetosave->MoveToFirstObject();

	//Save components 
	for (std::vector<Component*>::iterator c = components.begin(); c != components.end(); ++c)
	{
		(*c)->OnSave(filetosave);
		filetosave->MoveToFirstObject();
	}

	//Save childs
	//for (std::vector<GameObject*>::iterator go = childrens.begin(); go != childrens.end(); ++go)
	//{
	//	(*go)->OnSave(filetosave);
	//}

	filetosave->MoveToFirstObject();


};

void GameObject::OnLoad(JSON_Doc* config)
{};

