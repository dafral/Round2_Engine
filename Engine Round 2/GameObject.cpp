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