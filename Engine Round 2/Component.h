#ifndef  __COMPONENT_H__
#define __COMPONENT_H__

#include "Application.h"
#include "GameObject.h"
#include <vector>

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/Quat.h"

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
	Component(ComponentType type) : type(type) {};
	virtual ~Component() {};

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
	ComponentTransform() : Component(TRANSFORM)
	{
		position = { 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };
		rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	};
	~ComponentTransform() {};

	const float3 GetPosition() { return position; };
	const float3 GetScale() { return scale; };
	const Quat GetRotation() { return rotation; };

	float3 position;
	Quat rotation;
	float3 scale;

	float4x4 transform;
};

class ComponentMesh : public Component
{
public:

	ComponentMesh() : Component(MESH) {};
	~ComponentMesh()
	{
		delete[] indices;
		delete[] vertices;
		delete[] texture_coords;
	};

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
	ComponentMaterial() : Component(MATERIAL) {};
	~ComponentMaterial() {};


	uint id_texture = 0;
};

#endif // ! __COMPONENT_H__

