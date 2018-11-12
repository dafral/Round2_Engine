#include "Globals.h"
#include "Application.h"
#include "SceneImporter.h"
#include "ModuleJSON.h"

#include "Component.h"
#include "Component_Transform.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Camera.h"

SceneImporter::SceneImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

SceneImporter::~SceneImporter()
{}

void SceneImporter::SaveScene(const char* path)
{

	JSON_Doc* new_json = App->json->LoadJSON(path);

	if (new_json == nullptr) 
	{
		new_json = App->json->CreateJSON(path);
	}
	else
	{
		new_json->CleanUp();
	}

	new_json->SetArray("GameObjects");
	new_json->SetArray("Components");

	// Saving all game objects
	for (std::vector<GameObject*>::iterator go = App->scene->gameobjects.begin(); go != App->scene->gameobjects.end(); ++go)
	{
		(*go)->OnSave(new_json);
	}

	CONSOLELOG("Scene saved! %d gameobjects saved", App->scene->gameobjects.size())

	new_json->Save();
}

void SceneImporter::LoadScene(const char* path)
{
	JSON_Doc* doc = App->json->LoadJSON(path);

	if (doc != nullptr)
	{
		//reset scene
		App->scene->CleanUp();
		App->scene->Start();

		for (int i = 0; i < doc->GetArraySize("GameObjects"); ++i)
		{
			doc->MoveToSectionFromArray("GameObjects", i);

			uint parent = doc->GetNumber("parent");
			std::string name = doc->GetString("name");
			uint id = doc->GetNumber("UID");
			bool visible = doc->GetBool("visible");
			bool is_static = doc->GetBool("is_static");
			//uint new_child_id = doc->GetNumber("new_child_id");
			if (parent == 0)
			{
				GameObject* go = App->scene->CreateGameObject(name, nullptr);

				go->SetUniqueID(id);
				go->SetVisible(visible);
				go->SetStatic(is_static);
				//go->SetNewChildId(new_child_id);
			}
			else
			{
				GameObject* go_parent = App->scene->GetGOByUniqueID(parent);
				GameObject* go = App->scene->CreateGameObject(name, go_parent);

				go_parent->AddChildren(go);
				go->SetVisible(visible);
				go->SetStatic(is_static);
				//go->addSetNewChildId(new_child_id);
			}

			doc->MoveToFirstObject();
		}

		//LOAD COMPONENTS
		for (int i = 0; i < doc->GetArraySize("Components"); ++i)
		{
			doc->MoveToSectionFromArray("Components", i);

			uint owner = doc->GetNumber("owner");
			GameObject* go = App->scene->GetGOByUniqueID(owner);

			switch ((int)doc->GetNumber("type"))
			{
			case Component_Type::TRANSFORM:
			{
				float3 pos = doc->GetNumber3("position");
				float3 scale = doc->GetNumber3("scale");
				float4 rot = doc->GetNumber4("rotation");
				Quat rotation(rot.x, rot.y, rot.z, rot.w);

				Component_Transform * transform = (Component_Transform*)go->FindComponentWithType(TRANSFORM);
				transform->SetPosition(pos);
				transform->SetRotation(rotation);
				transform->SetScale(scale);

				break;
			}		
			}

			doc->MoveToFirstObject();
		}
	}
}

