#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include <vector>

class Component_Material;
class GameObject;

class ModuleMaterial : public Module
{
public:
	ModuleMaterial(Application* app, bool start_enabled = true);
	~ModuleMaterial();

	bool Start();
	bool CleanUp();

	void LoadTexture(const char* path, GameObject* game_obj);
	void DeleteTextures();

	Component_Material* CreateComponentMaterial(GameObject* my_go);

private:
	std::vector<Component_Material*> materials;

};