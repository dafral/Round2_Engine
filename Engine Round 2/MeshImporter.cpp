#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MeshImporter.h"

Component_Mesh* MeshImporter::Load(const char * filepath)
{
	//Open the file and get the size
	FILE* file = fopen(filepath, "rb");
	fseek(file, 0, SEEK_END);
	uint size = ftell(file);
	rewind(file);

	// Create a buffer to get the data of the file
	char* buffer = new char[size];
	char* cursor = buffer;

	// Read the file and close it
	fread(buffer, sizeof(char), size, file);
	fclose(file);

	// Copy unique id
	double* id = new double;
	uint bytes = sizeof(double);
	memcpy(id, cursor, bytes);
	cursor += bytes;

	// Copy the ranges
	uint ranges[3];		// ranges[0] = Vertices, ranges[1] = Indices, ranges[2] = Uvs
	bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	// Store indices
	uint* indices = new uint[ranges[1]];
	bytes = sizeof(uint) * ranges[1];
	memcpy(indices, cursor, bytes);
	cursor += bytes;

	// Store vertices
	float* vertices = new float[ranges[0] * 3];
	bytes = sizeof(float) * ranges[0] * 3;
	memcpy(vertices, cursor, bytes);
	cursor += bytes;

	// Store UVs
	float* uvs = new float[ranges[2] * 3];
	bytes = sizeof(float) * ranges[2] * 3;
	memcpy(uvs, cursor, bytes);
	cursor += bytes;

	// Create mesh --------------
	Component_Mesh* new_mesh = new Component_Mesh;

	new_mesh->SetFaces(vertices, ranges[0], indices, ranges[1]);
	new_mesh->SetUvs(uvs, ranges[2]);
	new_mesh->LoadToMemory();
	App->renderer3D->GetMeshesVector()->push_back(new_mesh);

	CONSOLELOG("New mesh with %d vertices", ranges[0] * 3);
	CONSOLELOG("New mesh with %d indices", ranges[1]);

	RELEASE(id);
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(uvs);

	return new_mesh;
}

bool MeshImporter::Save(const char * path, Component_Mesh* mesh)
{
	bool ret = true;

	//
	std::string name = "prueba";

	uint ranges[3] = { mesh->GetNumVertices(), mesh->GetNumIndices(), mesh->GetNumUVs() };
	uint size = sizeof(double) + sizeof(ranges) +
		sizeof(uint) * mesh->GetNumIndices() +
		sizeof(float) * mesh->GetNumVertices() * 3 +
		sizeof(float) * mesh->GetNumUVs() * 3;

	// Allocate data
	char* data = new char[size];
	char* cursor = data;

	// Store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * mesh->GetNumIndices();
	memcpy(cursor, mesh->GetIndices(), bytes);
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float) * mesh->GetNumVertices() * 3;
	memcpy(cursor, mesh->GetVertices(), bytes);
	cursor += bytes;

	// Store UVs
	bytes = sizeof(float) * mesh->GetNumUVs() * 3;
	memcpy(cursor, mesh->GetTexCoords(), bytes);

	//fopen
	if (App->filesystem->SaveFile(path, data, name.c_str(), "mymesh", size) == false)
	{
		return false;
	}

	RELEASE_ARRAY(data);

	return ret;
}

