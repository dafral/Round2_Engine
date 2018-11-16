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
update_status ModuleScene::PreUpdate(float dt)
{
	root_node->Update();

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

// This fcn test if a given ray collides with all the Game Objects of the scene.
void ModuleScene::RayCollision(LineSegment ray)
{
	/*GameObject* selected = nullptr;
	std::vector<GameObject*> possible_collisions;

	App->octree->RayIntersections(ray, possible_collisions);

	for (uint i = 0; i < possible_collisions.size(); i++)
	{
		Component_Mesh* mesh = (Component_Mesh*)possible_collisions[i]->FindComponent(COMPONENT_MESH);

		if (mesh != nullptr)
		{
			Component_Transform* trans = (Component_Transform*)possible_collisions[i]->FindComponent(COMPONENT_TRANSFORM);

			LineSegment local_ray(ray);
			local_ray.Transform(trans->GetTransform().Inverted());

			for (uint j = 0; j < mesh->num_indices - 9; j += 3)
			{
				Triangle triangle;

				triangle.a.Set(mesh->vertices[mesh->indices[j]], mesh->vertices[mesh->indices[j + 1]], mesh->vertices[mesh->indices[j + 2]]);
				triangle.b.Set(mesh->vertices[mesh->indices[j + 3]], mesh->vertices[mesh->indices[j + 4]], mesh->vertices[mesh->indices[j + 5]]);
				triangle.c.Set(mesh->vertices[mesh->indices[j + 6]], mesh->vertices[mesh->indices[j + 7]], mesh->vertices[mesh->indices[j + 8]]);

				float dist;
				float3 hit_point;

				if (local_ray.Intersects(triangle, &dist, &hit_point))
				{
					if (dist < distance)
					{
						distance = dist;
						selected = possible_collisions[i];
					}
				}
			}
		}
	}

	App->imgui->hierarchy->go_selected = selected;
	distance = FLOAT_INF;*/
}




