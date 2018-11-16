#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"

#include <string>
#include <vector>

class GameObject;

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();

	void Draw();
	GameObject* CreateGameObject(std::string name, GameObject* parent);
	GameObject* GetGOByUniqueID(uint uid) const;
	void RayCollision(LineSegment ray);

public:	

	GameObject* root_node = nullptr;
	std::vector<GameObject*> gameobjects;
};

#endif

