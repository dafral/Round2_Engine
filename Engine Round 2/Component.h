#ifndef  __COMPONENT_H__
#define __COMPONENT_H__

#include "Application.h"
#include "GameObject.h"
#include <vector>

class GameObject;

enum ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL
};

class Component
{
public:
	void virtual Enable() {};
	void virtual Update() {};
	void virtual Disable() {};

	ComponentType type;
	bool active = true;
	GameObject* my_go;
};

class ComponentTransform : public Component
{
public:
	//float3 position;
	//float3 rotation;
	//float3 scale;
};

class ComponentMesh : public Component
{
public:

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

class ComponentMaterial : public Component
{
public:
	uint id_texture = 0;
};

#endif // ! __COMPONENT_H__

