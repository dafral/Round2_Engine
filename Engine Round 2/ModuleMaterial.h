#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include <vector>

class ComponentMaterial;

class ModuleMaterial : public Module
{
public:
	ModuleMaterial(Application* app, bool start_enabled = true);
	~ModuleMaterial();

	bool Start();
	/*update_status Update(float dt);*/
	bool CleanUp();

	void LoadTexture(const char* path);
	void DeleteTextures();

	ComponentMaterial* CreateComponentMaterial();

	void Draw();

private:

	std::vector<ComponentMaterial*> comp_mats;
};