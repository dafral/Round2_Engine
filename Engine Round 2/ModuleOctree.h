#ifndef __MODULE_OCTREE_H__
#define __MODULE_OCTREE_H__

#include "Module.h"
#include <vector>
#include <array>

class GameObject;
class Component_Camera;

#define MAX_OBJ_IN_NODE 6
#define MAX_DIVISIONS 100
#define SUBDIVISIONS 8 // Only number suported

class Octree_Node
{
public:
	Octree_Node(AABB box) : box(box) {};
	~Octree_Node() {};

	void Draw();
	void CleanUp();
	void FrustumIntersections(Component_Camera* curr_camera);
	void CollectIntersections(LineSegment ray, std::vector<GameObject*> &go_collided);

	void DivideNode();
	const bool IsFull() { return objects_in_node.size() > MAX_OBJ_IN_NODE; };

private:
	AABB box;
	bool divided = false;
	uint divisions = 0;
	Octree_Node* childs[SUBDIVISIONS];

public:
	std::vector<GameObject*> objects_in_node;
};

class ModuleOctree : public Module
{
public:
	ModuleOctree(Application* app, bool start_enabled = true) : Module(app, start_enabled) { root_node = nullptr; };
	~ModuleOctree() {};

	update_status Update(float dt);

	void AddStatic(GameObject* static_go);
	void RemoveStatic(GameObject* static_go);
	void StartOctree();
	void RayIntersections(LineSegment ray, std::vector<GameObject*> &go_collided);

	bool CleanUp();

private:
	Octree_Node* root_node = nullptr;
	std::vector<GameObject*> all_static_go;

public:
	uint divisions = 0;
};

#endif
