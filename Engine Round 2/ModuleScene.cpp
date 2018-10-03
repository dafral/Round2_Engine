#include "Globals.h"
#include "ModuleScene.h"
#include "Application.h"
#include "ModuleScene.h"
#include "myPrimitives.h"

#include "glew\include\glew.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	CONSOLELOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(15.0f, 45.0f, 15.0f));
	App->camera->LookAt(vec3(0, 0, 0));

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

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}

void ModuleScene::Draw()
{
	// Plane
	prPlane p(0, 1, 0, 0);
	p.axis = false;
	p.wire = false;
	p.color = Green;
	p.Render();

	// Cube
	myCube cube;
	cube.DrawCubeElements();
}



