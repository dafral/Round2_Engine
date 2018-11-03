#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Module.h"
#include "GameObject.h"
#include "Component.h"
#include "Component_Material.h"
#include <vector>

class Component_Material;
class GameObject;

class MaterialImporter : public Module
{
public:
	MaterialImporter(Application* app, bool start_enabled = true);
	~MaterialImporter();

	bool Start();
	bool CleanUp();

	void Import(const char* full_path, GameObject* go);
	Component_Material* CreateComponentMaterial(GameObject* my_go);
	void DeleteTextures();
	void SaveAsDDS();

	

private:
	std::vector<Component_Material*> materials;
	int save_id = 0;
};

#endif // !__MATERIALIMPORTER_H__

