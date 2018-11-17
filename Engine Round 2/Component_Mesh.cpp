#include "Application.h"
#include "Component_Mesh.h"
#include "GameObject.h"

#include "PCG/pcg_basic.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

Component_Mesh::Component_Mesh() : Component(MESH)
{
	unique_id = pcg32_random_r(&App->rng);
}

void Component_Mesh::SetFaces(aiMesh* new_mesh)
{
	// Vertices
	mesh.num_vertices = new_mesh->mNumVertices;
	mesh.vertices = new float[mesh.num_vertices * 3];
	memcpy(mesh.vertices, new_mesh->mVertices, sizeof(float) * mesh.num_vertices * 3);

	// Indices
	if (new_mesh->HasFaces())
	{
		mesh.num_indices = new_mesh->mNumFaces * 3;
		mesh.indices = new uint[mesh.num_indices];

		for (uint i = 0; i < new_mesh->mNumFaces; i++)
		{
			if (new_mesh->mFaces[i].mNumIndices != 3)
			{
				CONSOLELOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&mesh.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	// Bounding volume
	AABB box;
	box.SetNegativeInfinity();
	box.Enclose((float3*) new_mesh->mVertices, new_mesh->mNumVertices);
	bv.bounding_box = box;

	Sphere sph;
	sph.SetNegativeInfinity();
	sph.r = 0;
	sph.Enclose((float3*)new_mesh->mVertices, new_mesh->mNumVertices);
	bv.sphere = sph;
}

void Component_Mesh::SetUVs(aiMesh* new_mesh)
{
	// UVs
	if (new_mesh->HasTextureCoords(0))
	{
		mesh.num_uvs = new_mesh->mNumVertices;
		mesh.texture_coords = new float[mesh.num_uvs * 3];
		memcpy(mesh.texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * mesh.num_uvs * 3);
	}
}

void Component_Mesh::SetIDs(Component_Mesh* cmesh)
{
	mesh.id_indices = cmesh->mesh.id_indices;
	mesh.id_vertices = cmesh->mesh.id_vertices;
	mesh.id_uvs = cmesh->mesh.id_uvs;
}

void Component_Mesh::LoadBuffers(aiMesh* new_mesh)
{

	// Load buffer for vertices
	glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);

	// Load buffer for indices
	if (new_mesh->HasFaces())
	{
		glGenBuffers(1, (GLuint*) &(mesh.id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);
	}

	// Load buffer for UVs
	if (new_mesh->HasTextureCoords(0))
	{
		glGenBuffers(1, (GLuint*) &(mesh.id_uvs));
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)mesh.id_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh.num_uvs * 3, mesh.texture_coords, GL_STATIC_DRAW);
	}
}

// -------------------------------------------------------------
//Own Format

void Component_Mesh::SetFaces(float* n_vertices, uint n_num_vertices, uint* n_indices, uint n_num_indices)
{
	if (n_num_vertices > 0)
	{
		// Vertices
		mesh.vertices = new float[n_num_vertices * 3];
		memcpy(mesh.vertices, n_vertices, sizeof(float) * n_num_vertices * 3);
		mesh.num_vertices = n_num_vertices;

		if (n_num_indices > 0)
		{
			// Indices
			mesh.num_indices = n_num_indices;
			mesh.indices = new uint[n_num_indices];
			if (n_num_indices != 3)
			{
				CONSOLELOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(mesh.indices, n_indices, sizeof(uint) * n_num_indices);
			}
		}
	}
}

void Component_Mesh::SetUvs(float* n_uvs, uint n_num_uvs)
{
	if (n_num_uvs > 0)
	{
		// UVs
		mesh.texture_coords = new float[n_num_uvs * 3];
		memcpy(mesh.texture_coords, n_uvs, sizeof(float) * n_num_uvs * 3);
		mesh.num_uvs = n_num_uvs;
	}
}

void Component_Mesh::LoadToMemory()
{
	if (mesh.id_vertices == 0 && mesh.vertices != nullptr)
	{
		uint id = 0;

		glGenBuffers(1, (GLuint*)&(id));
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (mesh.id_indices == 0 && mesh.indices != nullptr)
	{
		uint id = 0;

		glGenBuffers(1, (GLuint*)&(id));
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (mesh.id_uvs == 0 && mesh.texture_coords != nullptr)
	{
		uint id = 0;

		glGenBuffers(1, (GLuint*)&(id));
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_uvs * 3, mesh.texture_coords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

// -------------------------------------------------------------

void Component_Mesh::OnSave(JSON_Doc* filetosave)
{
	filetosave->AddSectionToArray("Components");
	filetosave->MoveToSectionFromArray("Components", filetosave->GetArraySize("Components") - 1);

	filetosave->SetNumber("type", 1);
	filetosave->SetNumber("owner",my_go->GetUniqueID());

	filetosave->SetNumber("mesh", GetUniqueID());
	filetosave->SetNumber("mesh", 0);
}
