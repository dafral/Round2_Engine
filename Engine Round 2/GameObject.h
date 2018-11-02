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
	void AddComponent(Component* component);
	Component* FindComponentWithType(Component_Type type);
	void SetStatic(bool new_static) { is_static = new_static; };
	void SetVisible(bool new_visible) { is_visible = new_visible; };
	bool GetStatic() { return is_static; };
	bool GetVisible() { return is_visible; };

public:

	std::string name;
	GameObject* parent;

	std::vector<GameObject*> childrens;
	std::vector<Component*> components;

private:
	bool is_visible = true;
	bool is_static = false;
};

#endif // ! __GAMEOBJECT_H__
