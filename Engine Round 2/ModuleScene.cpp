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

	App->camera->Move(vec3(0.0f, 10.0f, 10.0f));
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

	// Cube
	myCube cube;
	cube.Draw();

	/*
	// Sphere
	mySphere sphere(4.0f, 12, 24);
	sphere.Draw();
	*/

	// Plane
	myPlane plane(12.0f);
	plane.Draw();
}



