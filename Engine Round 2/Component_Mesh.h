#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Globals.h"
#include "Component.h"
#include "ModuleJSON.h"
#include "myPrimitives.h"
#include "MathGeoLib/MathGeoLib.h"

struct aiMesh;

struct Bounding_Volume
{
	Sphere sphere;
	AABB bounding_box;
};

struct Mesh
{
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

// 0..1
class Component_Mesh : public Component
{
public: 
	Component_Mesh();
	virtual ~Component_Mesh() {};

private:
	Mesh mesh;
	uint unique_id = 0;
	Bounding_Volume bv;

public:
	uint GetIdVertices() { return mesh.id_vertices; };
	uint GetNumVertices() { return mesh.num_vertices; };
	float* GetVertices() { return mesh.vertices; };
	
	uint GetIdIndices() { return mesh.id_indices; };
	uint GetNumIndices() { return mesh.num_indices; };
	uint* GetIndices() { return mesh.indices; };

	uint GetIdUVs() { return mesh.id_uvs; };
	uint GetNumUVs() { return mesh.num_uvs; };
	float* GetTexCoords() { return mesh.texture_coords; };

	uint GetUniqueID() { return unique_id; };
	Bounding_Volume GetBoundingVolume() { return bv; };

	void SetFaces(aiMesh* new_mesh);
	void SetUVs(aiMesh* new_mesh);
	void SetIDs(Component_Mesh* cmesh);
	void LoadBuffers(aiMesh* new_mesh);

	void OnSave(JSON_Doc* filetosave);
};

#endif
