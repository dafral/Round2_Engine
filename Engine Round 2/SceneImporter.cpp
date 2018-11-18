#include "Globals.h"
#include "Application.h"
#include "SceneImporter.h"
#include "ModuleJSON.h"

#include "Component.h"
#include "Component_Transform.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Camera.h"

#include "MaterialImporter.h"
#include "MeshImporter.h"

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

		App->scene->state = EDITOR;
		App->scene->game_clock.Start();
		App->scene->game_clock.Stop();

		for (int i = 0; i < doc->GetArraySize("GameObjects"); ++i)
		{
			doc->MoveToSectionFromArray("GameObjects", i);

			uint parent = doc->GetNumber("parent");
			std::string name = doc->GetString("name");
			uint id = doc->GetNumber("unique ID");
			bool visible = doc->GetBool("visible");
			bool is_static = doc->GetBool("is_static");

			if (parent == 0)
			{
				GameObject* go = App->scene->CreateGameObject(name, nullptr);

				go->SetUniqueID(id);
				go->SetVisible(visible);
				go->SetStatic(is_static);

				if (doc->GetBool("root"))
					App->scene->SetRootNode(go);
			}
			else
			{
				GameObject* go_parent = App->scene->GetGOByUniqueID(parent);
				GameObject* go = App->scene->CreateGameObject(name, go_parent);

				go_parent->AddChildren(go);
				go->SetUniqueID(id);
				go->SetVisible(visible);
				go->SetStatic(is_static);
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
			case Component_Type::MESH:
			{
				uint mesh_uniqueid = doc->GetNumber("mesh");
				std::string mesh_path = App->filesystem->library_mesh_path.c_str();
				mesh_path += std::to_string(mesh_uniqueid);
				mesh_path += ".mymesh";

				go->AddComponent(App->mesh_importer->Load(mesh_path.c_str()));

				Component_Mesh * mesh = (Component_Mesh*)go->FindComponentWithType(MESH);
				mesh->SetUniqueID(doc->GetNumber("mesh"));

				break;
			}
			case Component_Type::MATERIAL:
			{
				uint material_uniqueid = doc->GetNumber("material");
				std::string material_path = App->filesystem->library_material_path.c_str();
				material_path += std::to_string(material_uniqueid);
				material_path += ".dds";

				App->material_importer->Import(material_path.c_str(), go);
				break;
			}
			case Component_Type::CAMERA:
			{
				float aspect_ratio = doc->GetNumber("aspect_ratio");
				float vertical_fov = doc->GetNumber("vertical_fov");
				float near_plane = doc->GetNumber("near");
				float far_plane = doc->GetNumber("far");

				Component_Camera* cam = new Component_Camera();				
				cam->SetFOV(vertical_fov);
				cam->SetAspectRatio(aspect_ratio);
				cam->SetPlanes(near_plane, far_plane);

				go->AddComponent(cam);

				if (doc->GetBool("game_camera"))
					App->camera->SetGameCamera(cam);
				if (doc->GetBool("scene_camera"))
					App->camera->SetSceneCamera(cam);

				break;
			}
			}

			doc->MoveToFirstObject();
		}
	}
}

