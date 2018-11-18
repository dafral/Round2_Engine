#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "ModuleJSON.h"
#include <string>
#include <vector>

class Component;
class Component_Mesh;

class GameObject
{
public:

	GameObject(std::string name, GameObject* parent);
	~GameObject();

	void Update();
	void CleanUp();
	void Draw(bool is_scene_camera);
	void DrawDebug(Component_Mesh* mesh);
	
	void AddChildren(GameObject* children); 
	void AddComponent(Component* component);
	Component* FindComponentWithType(Component_Type type);

	void SetStatic(bool new_static);
	void SetVisible(bool new_visible);
	void SetUniqueID(uint id);

	bool GetStatic() { return is_static; };
	bool GetVisible() { return is_visible; };
	uint GetUniqueID() { return unique_id; };
	void GetChildrenByUniqueID(uint UID, GameObject *& go) const;

	void OnSave(JSON_Doc* config);
	void OnLoad(JSON_Doc* config);

public:
	std::string name;
	GameObject* parent;

	std::vector<GameObject*> childrens;
	std::vector<Component*> components;

private:
	bool is_visible = true;
	bool is_static = false;
	uint unique_id;
};

#endif // ! __GAMEOBJECT_H__
