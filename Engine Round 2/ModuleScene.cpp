#include "Application.h"
#include "ModuleScene.h"
#include "myPrimitives.h"

#include "Component.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	CONSOLELOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(0.0f, 10.0f, 10.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	root_node = CreateGameObject("root", nullptr);

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
	// HERE
	
	return new_go;
}



