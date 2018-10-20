#ifndef  __COMPONENT_H__
#define __COMPONENT_H__

#include "Application.h"
#include "GameObject.h"
#include <vector>

struct Texture
{
	uint id_texture = 0;
};

struct Mesh
{
	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_uvs = 0; // id in VRAM
	uint num_uvs = 0;
	float* texture_coords = nullptr;
};

enum ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL
};

class Component
{
public:
	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	ComponentType type;
	bool active = true;
	GameObject* my_go;
};

class ComponentTransform : public Component
{
public:
	float3 position;
	float3 rotation;
	float3 scale;
};

class ComponentMesh : public Component
{
public:
	std::vector<Mesh*> meshes;
};

class ComponentMaterial : public Component
{
public:
	Texture* texture;
};

#endif // ! __COMPONENT_H__

