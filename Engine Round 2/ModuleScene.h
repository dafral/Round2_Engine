#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>
#include <vector>

class GameObject;

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw();
	GameObject* CreateGameObject(std::string name, GameObject* parent);

public:	

	GameObject* root_node = nullptr;
	std::vector<GameObject*> gameobjects;
};

