#pragma once

#include "Globals.h"
#include "Component.h"

struct aiMesh;

// 0..1
class Component_Mesh : public Component
{
public: 
	Component_Mesh() : Component(MESH) {};
	virtual ~Component_Mesh() {};

private:
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* texture_coords = nullptr;

public:
	uint GetIdVertices() { return id_vertices; };
	uint GetNumVertices() { return num_vertices; };
	float* GetVertices() { return vertices; };
	
	uint GetIdIndices() { return id_indices; };
	uint GetNumIndices() { return num_indices; };
	uint* GetIndices() { return indices; };

	uint GetIdUVs() { return id_uvs; };
	uint GetNumUVs() { return num_uvs; };
	float* GetTexCoords() { return texture_coords; };

	void SetFaces(aiMesh* new_mesh);
	void SetUVs(aiMesh* new_mesh);
	void SetIDs(Component_Mesh* cmesh);
	void LoadBuffers(aiMesh* new_mesh);
};
