#include "Component_Mesh.h"
#include "glew/include/glew.h"

float* Component_Mesh::GetVertices()
{
	return vertices;
}

uint Component_Mesh::GetIdVertices()
{
	return id_vertices;
}

uint Component_Mesh::GetNumVertices()
{
	return num_vertices;
}

uint* Component_Mesh::GetIndices()
{
	return indices;
}

uint Component_Mesh::GetIdIndices()
{
	return id_indices;
}

uint Component_Mesh::GetNumIndices()
{
	return num_indices;
}

float* Component_Mesh::GetTexCoords()
{
	return texture_coords;
}

uint Component_Mesh::GetIdUVs()
{
	return id_uvs;
}

uint Component_Mesh::GetNumUVs()
{
	return num_uvs;
}

void Component_Mesh::SetFaces(float* n_vertices, uint n_num_vertices, uint* n_indices, uint n_num_indices)
{
	if (n_num_vertices > 0)
	{
		// Vertices
		vertices = new float[n_num_vertices * 3];
		memcpy(vertices, n_vertices, sizeof(float) * n_num_vertices * 3);
		num_vertices = n_num_vertices;

		if (n_num_indices > 0)
		{
			// Indices
			num_indices = n_num_indices;
			indices = new uint[n_num_indices];
			memcpy(indices, n_indices, sizeof(uint) * n_num_indices);		
		}
	}
}

void Component_Mesh::SetUvs(float* n_uvs, uint n_num_uvs)
{
	if (n_num_uvs > 0)
	{
		// UVs
		texture_coords = new float[n_num_uvs * 3];
		memcpy(texture_coords, n_uvs, sizeof(float) * n_num_uvs * 3);
		num_uvs = n_num_uvs;
	}
}

void Component_Mesh::LoadToMemory()
{
	if (id_vertices == 0 && vertices != nullptr) 
	{
		glGenBuffers(1, (GLuint*)&(id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	}

	if (id_indices == 0 && indices != nullptr)
	{
		glGenBuffers(1, (GLuint*)&(id_indices));
		glBindBuffer(GL_ARRAY_BUFFER, id_indices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_indices, indices, GL_STATIC_DRAW);
	}

	if (id_uvs == 0 && texture_coords != nullptr)
	{
		glGenBuffers(1, (GLuint*)&(id_uvs));
		glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_uvs * 3, texture_coords, GL_STATIC_DRAW);
	}
}