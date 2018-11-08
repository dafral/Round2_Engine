#include "Globals.h"
#include "Application.h"
#include "SceneImporter.h"
#include "ModuleJSON.h"

SceneImporter::SceneImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

SceneImporter::~SceneImporter()
{}

void SceneImporter::SaveScene(const char* path)
{
	JSON_Doc* new_json = App->json->LoadJSON(path);
	if (new_json == nullptr)
		new_json = App->json->CreateJSON(path);
	else
		new_json->CleanUp();
	new_json->SetArray("GameObjects");
	new_json->SetArray("Components");

	// Saving all game objects
	for (std::vector<GameObject*>::iterator go = App->scene->gameobjects.begin(); go != App->scene->gameobjects.end(); ++go)
	{
		(*go)->OnSave(new_json);
	}

	new_json->Save();
}

