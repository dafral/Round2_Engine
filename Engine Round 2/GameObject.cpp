#include "GameObject.h"

GameObject::GameObject()
{}

GameObject::~GameObject()
{}

Component* GameObject::CreateComponent(ComponentType type)
{
	switch (type) 
	{

		case TRANSFORM:
		{
			return nullptr;
		}

		case MESH:
		{
			return App->renderer3D->CreateComponentMesh();
		}

		case MATERIAL:
		{
			return App->material->CreateComponentMaterial();
		}		

	}
}