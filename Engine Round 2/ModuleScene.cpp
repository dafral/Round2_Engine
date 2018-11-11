#include "Application.h"
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



