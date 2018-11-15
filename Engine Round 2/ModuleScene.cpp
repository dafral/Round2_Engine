#include "Application.h"
#include "Globals.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "myPrimitives.h"
#include "Component.h"
#include "Component_Transform.h"
#include <algorithm>

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	CONSOLELOG("Loading Intro assets");
	bool ret = true;

	root_node = CreateGameObject("root", nullptr);
	App->camera->SetSceneCamera(App->camera->CreateComponentCamera("Scene Camera"));
	App->camera->SetGameCamera(App->camera->CreateComponentCamera("Main Camera"));

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	CONSOLELOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{
	if (state == PLAY || state == PAUSED || state == TICK)
	{
		if (state == PAUSED)
			game_dt = 0.f;

		//calculate game_dt
		game_dt = game_clock.ReadSec() - last_game_frame_time;
		last_game_frame_time = game_clock.ReadSec();

		if (state == TICK)
		{
			state = EDITOR;
			CONSOLELOG("Game Logic has done ONE tick!");
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleScene::Draw()
{
	// Plane
	myPlane plane(12.0f);
	plane.Draw();
}

// ===========================================================================
// Game objects
// ===========================================================================

GameObject* ModuleScene::CreateGameObject(std::string name, GameObject* parent)
{
	GameObject* new_go = new GameObject(name, parent);

	if (parent != nullptr) 
		parent->AddChildren(new_go);

	// We have to create a component transform by default 
	Component_Transform* trans = new Component_Transform();
	trans->my_go = new_go;
	new_go->AddComponent(trans);

	gameobjects.push_back(new_go);

	return new_go;
}

GameObject* ModuleScene::GetGOByUniqueID(uint uid) const
{
	GameObject* ret = nullptr;

	for (int i = 0; i < gameobjects.size(); ++i)
	{
		if (gameobjects[i]->GetUniqueID() == uid)
		{
			ret = gameobjects[i];
			break;
		}
		else
		{
			gameobjects[i]->GetChildrenByUniqueID(uid, ret);

			if (ret != nullptr)
				break;
		}
	}

	return ret;
}

// ===========================================================================
// Time Management
// ===========================================================================

void ModuleScene::Play()
{
	if (state == PAUSED || state == EDITOR)
	{
		state = PLAY;
		game_clock.Start();
		App->scene_importer->SaveScene(App->filesystem->scene_path.c_str());
		CONSOLELOG("%d", state);
	}
	else if (state == PLAY)
	{
		state = EDITOR;
		game_clock.Stop();
		App->scene_importer->LoadScene(App->filesystem->scene_path.c_str());
		CONSOLELOG("%d", state);
	}
}

void ModuleScene::Pause()
{
	if (state == PLAY || state == PAUSED)
	{
		if (state == PLAY)
		{
			state = PAUSED;
			game_clock.PauseOn();
		}
		else
		{
			state = PLAY;
			game_clock.PauseOff();
		}
	}
}

void ModuleScene::Tick()
{
	state = TICK;
}

float ModuleScene::ReadTimer() const
{
	return game_clock.ReadSec();
}

