#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Timer.h"

#include <string>
#include <vector>

enum SceneState
{
	EDITOR,
	PAUSED,
	PLAY,
	TICK
};

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
	void RayCast(LineSegment ray);
	void RayIntersections(LineSegment ray, std::vector<GameObject*> &go_collided);

	// Time management ---------------
	void Play();
	void Pause();
	void Tick();
	float ReadTimer() const;

public:		
	GameObject* root_node = nullptr;
	std::vector<GameObject*> gameobjects;

	Timer game_clock;
	SceneState state;
	SceneState prev_tick_state;
	float last_game_frame_time = 0.f;
	float game_dt = 0.f;
};

#endif

