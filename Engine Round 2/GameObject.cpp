#include "GameObject.h"

GameObject::GameObject()
{}

GameObject::~GameObject()
{}

void GameObject::AddComponent(Component* component)
{
	if (component != nullptr)
	{
		components.push_back(component);
	}
}

const Component* GameObject::FindComponentWithType(ComponentType type)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type) return components[i];
	}
	return nullptr;
}

// This function deletes all components of that type
void GameObject::DeleteComponentWithType(ComponentType type)
{
	for (int i = 0; i < components.size(); )
	{
		if (components[i]->type == type)
		{
			delete components[i];
			components.erase(components.begin() + i);
		}
		else i++;
	}
}