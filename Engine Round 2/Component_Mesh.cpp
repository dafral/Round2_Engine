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

void Component_Mesh::OnSave(JSON_Doc* filetosave)
{
	filetosave->AddSectionToArray("Components");
	filetosave->MoveToSectionFromArray("Components", filetosave->GetArraySize("Components") - 1);

	filetosave->SetNumber("type", 1);
	filetosave->SetNumber("owner",my_go->GetUniqueID());

	filetosave->SetNumber("mesh", GetUniqueID());
	filetosave->SetNumber("mesh", 0);
}