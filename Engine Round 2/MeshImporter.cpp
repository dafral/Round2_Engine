#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MeshImporter.h"
#include "Component_Material.h"
#include "Component_Transform.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

MeshImporter::MeshImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
MeshImporter::~MeshImporter()
{}

void MeshImporter::ImportScene(const char* full_path, const char* file_name)
{
	// Creating the parent (empty game object) 
	GameObject* empty_go = App->scene->CreateGameObject(file_name, App->scene->root_node);

	// Loading scene
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	aiNode* node = scene->mRootNode;

	if (scene != nullptr && scene->HasMeshes())
	{
		ImportMesh(empty_go, scene, node);
		CONSOLELOG("Scene %s loaded.", full_path);
	}
	else
	{
		CONSOLELOG("Error loading scene %s.", full_path);
	}

	// Releasing scene
	aiReleaseImport(scene);
}

void MeshImporter::ImportMesh(GameObject* parent, const aiScene* scene, aiNode* node)
{
	if (node->mNumMeshes <= 0)
	{
		// Recursion
		for (int i = 0; i < node->mNumChildren; i++)
			ImportMesh(parent, scene, node->mChildren[i]);
	}
	else
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[i]];

			// Creating a Game Object (child of parent) for each mesh.
			GameObject* go = App->scene->CreateGameObject(node->mName.C_Str(), parent);
			Component_Mesh* cmesh = App->renderer3D->CreateComponentMesh(go);

			// Setting values cmesh
			cmesh->SetFaces(new_mesh);
			cmesh->SetUVs(new_mesh);

			// Changing transform
			Component_Transform* trans = (Component_Transform*)go->FindComponentWithType(TRANSFORM);

			if (node != nullptr)
			{
				aiVector3D translation;
				aiVector3D scaling;
				aiQuaternion rotation;

				node->mTransformation.Decompose(scaling, rotation, translation);

				float3 pos(translation.x, translation.y, translation.z);
				float3 scale(scaling.x, scaling.y, scaling.z);
				Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

				trans->SetPosition(pos);
				trans->SetRotation(rot);
				trans->SetScale(scale);
			}

			// Check if we already loaded this mesh in memory
			Component_Mesh* aux = App->renderer3D->IsMeshLoaded(cmesh);

			if (App->renderer3D->GetMeshesVector()->size() == 1 || aux == nullptr) cmesh->LoadBuffers(new_mesh);
			else cmesh->SetIDs(aux);

			// Import mesh HERE
			App->mesh_importer->Save(App->filesystem->library_mesh_path.c_str(), cmesh);

			// Recursion
			for (int i = 0; i < node->mNumChildren; i++)
				ImportMesh(go, scene, node->mChildren[i]);
		}
	}
}


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

	// Copy the ranges
	uint ranges[3];		// ranges[0] = Vertices, ranges[1] = Indices, ranges[2] = Uvs
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	//Copy the ids    
	uint ids[3];		// ids[0] = Vertices, ids[1] = Indices, ids[2] = Uvs
	bytes = sizeof(ids);
	memcpy(ids, cursor, bytes);
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
	new_mesh->SetIDs(ids[0], ids[1], ids[2]);
	new_mesh->LoadToMemory();

	App->renderer3D->GetMeshesVector()->push_back(new_mesh);
	
	CONSOLELOG("Loading mesh with %d vertices", new_mesh->GetNumVertices());
	CONSOLELOG("Loading mesh with %d indices", new_mesh->GetNumIndices());

	/*RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(uvs);*/

	return new_mesh;
}

bool MeshImporter::Save(const char * path, Component_Mesh* mesh)
{
	bool ret = true;
	std::string name =	std::to_string(mesh->GetUniqueID()).c_str();
	
	uint ranges[3] = { mesh->GetNumVertices(), mesh->GetNumIndices(), mesh->GetNumUVs() };
	uint ids[3] = { mesh->GetIdVertices(), mesh->GetIdIndices(), mesh->GetIdUVs() };

	uint size = sizeof(double) + sizeof(ranges) + sizeof(ids) +
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
	
	//Store ids
	bytes = sizeof(ids);
	memcpy(cursor, ids, bytes);
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

	CONSOLELOG("New mesh saved with %d vertices", ranges[0]);
	CONSOLELOG("New mesh saved with %d indices", ranges[1]);

	RELEASE_ARRAY(data);

	return ret;
}


