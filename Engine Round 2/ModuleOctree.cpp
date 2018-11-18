#include "Application.h"
#include "ModuleOctree.h"
#include "GameObject.h"
#include "Color.h"
#include "Component_Camera.h"
#include "Component_Transform.h"

void ModuleOctree::AddStatic(GameObject* static_go)
{
	all_static_go.push_back(static_go);
}

void ModuleOctree::RemoveStatic(GameObject* static_go)
{
	for (int i = 0; i < all_static_go.size(); i++)
	{
		if (all_static_go[i] == static_go)
		{
			all_static_go.erase(all_static_go.begin() + i);
		}
	}
}

update_status ModuleOctree::Update(float dt)
{
	if (root_node != nullptr)
	{
		root_node->FrustumIntersections(App->camera->GetSceneCamera()); // GO to draw
		root_node->FrustumIntersections(App->camera->GetGameCamera());  // for each camera
		root_node->Draw();
	}

	return UPDATE_CONTINUE;
}

void ModuleOctree::StartOctree()
{
	// Setting root box parameters
	float3 min_point = { 0, 0, 0 };
	float3 max_point = { 0, 0, 0 };

	for (int i = 0; i < all_static_go.size(); i++)
	{
		Component_Mesh* mesh = (Component_Mesh*)all_static_go[i]->FindComponentWithType(MESH);
		Component_Transform* trans = (Component_Transform*)all_static_go[i]->FindComponentWithType(TRANSFORM);

		if (mesh != nullptr)
		{
			AABB bb = mesh->GetBoundingVolume().bounding_box;
			bb.TransformAsAABB(trans->GetGlobalTransform().Transposed());

			if (bb.minPoint.x < min_point.x)
				min_point.x = bb.minPoint.x;

			if (bb.minPoint.y < min_point.y)
				min_point.y = bb.minPoint.y;

			if (bb.minPoint.z < min_point.z)
				min_point.z = bb.minPoint.z;

			if (bb.maxPoint.x > max_point.x)
				max_point.x = bb.maxPoint.x;

			if (bb.maxPoint.y > max_point.y)
				max_point.y = bb.maxPoint.y;

			if (bb.maxPoint.z > max_point.z)
				max_point.z = bb.maxPoint.z;
		}
	}

	// Creating root node
	AABB root_box(min_point, max_point);
	root_node = new Octree_Node(root_box);

	// Slpit root node
	root_node->objects_in_node = all_static_go;
	root_node->DivideNode();
}

// ----------------------------------------------------------

void Octree_Node::DivideNode()
{
	if (IsFull() && App->octree->divisions <= MAX_DIVISIONS)
	{
		App->octree->divisions++;
		divided = true;

		// Creating 8 new nodes
		float3 new_size = box.HalfSize();
		float3 center = box.CenterPoint();
		int children_count = 0;

		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					float3 min_point(box.minPoint.x + z * new_size.x, box.minPoint.y + y * new_size.y, box.minPoint.z + x * new_size.z);
					float3 max_point(min_point.x + new_size.x, min_point.y + new_size.y, min_point.z + new_size.z);

					AABB new_box(min_point, max_point);

					childs[children_count] = new Octree_Node(new_box);
					children_count++;
				}
			}
		}

		// Reallocate the GOs from the parent to its childs
		for (int i = 0; i < SUBDIVISIONS; i++)
		{
			for (int j = 0; j < objects_in_node.size(); j++)
			{
				Component_Mesh* mesh = (Component_Mesh*)objects_in_node[j]->FindComponentWithType(MESH);
				Component_Transform* trans = (Component_Transform*)objects_in_node[j]->FindComponentWithType(TRANSFORM);

				if (mesh != nullptr)
				{
					AABB bb = mesh->GetBoundingVolume().bounding_box;
					bb.TransformAsAABB(trans->GetGlobalTransform().Transposed());

					if (childs[i]->box.Intersects(bb))
					{
						childs[i]->objects_in_node.push_back(objects_in_node[j]);
					}
				}
			}
		}
		objects_in_node.clear();

		// Recursion for childs
		for (int i = 0; i < SUBDIVISIONS; i++)
			childs[i]->DivideNode();
	}
}

// ----------------------------------------------------------

void Octree_Node::Draw()
{
	float3 vertices[8];
	box.GetCornerPoints(vertices);

	App->debug->DrawBox(vertices, 2.f, Yellow);

	if (divided)
	{
		for (int i = 0; i < SUBDIVISIONS; i++)
			childs[i]->Draw();
	}
}

// -------------------------------------------------------------

bool ModuleOctree::CleanUp()
{
	if (root_node != nullptr)
	{
		root_node->CleanUp();
		root_node = nullptr;
		divisions = 0;
	}

	return true;
}

void Octree_Node::CleanUp()
{
	objects_in_node.clear(); // We don't want to delete the GOs, only to clear the vector

	if (divided)
	{
		for (int i = 0; i < SUBDIVISIONS; i++)
			childs[i]->CleanUp();
	}

	delete this;
}

// ---------------------------------------------------------------

void Octree_Node::FrustumIntersections(Component_Camera* curr_camera)
{
	if (curr_camera->IsAABBInside(box))
	{
		// Add game objects to draw
		for (int i = 0; i < objects_in_node.size(); i++)
		{
			Component_Mesh* mesh = (Component_Mesh*)objects_in_node[i]->FindComponentWithType(MESH);
			Component_Transform* trans = (Component_Transform*)objects_in_node[i]->FindComponentWithType(TRANSFORM);

			if (mesh != nullptr)
			{
				AABB bb = mesh->GetBoundingVolume().bounding_box;
				bb.TransformAsAABB(trans->GetGlobalTransform().Transposed());
				Sphere sph = mesh->GetBoundingVolume().sphere;
				sph.Transform(trans->GetGlobalTransform().Transposed());

				if (objects_in_node[i]->GetVisible() && curr_camera->IsSphereInside(sph))
				{
					if (curr_camera->IsAABBInside(bb))
					{
						App->renderer3D->AddGOToDraw(objects_in_node[i], curr_camera);
					}
				}
			}
		}

		// Recursion
		if (divided)
		{
			for (int i = 0; i < SUBDIVISIONS; i++)
				childs[i]->FrustumIntersections(curr_camera);
		}
	}
}