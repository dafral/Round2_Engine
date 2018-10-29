#pragma once

#include "Globals.h"
#include "Component.h"

// 0..1
class Component_Mesh : public Component
{
public: 
	Component_Mesh() : Component(MESH) {};
	virtual ~Component_Mesh() {};

public:
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* texture_coords = nullptr;
};
