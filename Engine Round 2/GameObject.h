#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"
#include <string>
#include <vector>

class Component;

class GameObject
{
public:

	GameObject(std::string name, GameObject* parent);
	~GameObject();

	void AddChildren(GameObject* children);

public:

	std::string name;
	GameObject* parent;

	bool is_visible = true;
	bool is_static = true;

	std::vector<GameObject*> childrens;
};

#endif // ! __GAMEOBJECT_H__
