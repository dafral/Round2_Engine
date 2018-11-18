#include "Application.h"
#include "Globals.h"
#include "ModuleResources.h"
#include <algorithm>

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "SceneImporter.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleResources::~ModuleResources()
{}

uint ModuleResources::ImportFile(const char* new_file_in_assets)
{
	uint ret = 0; //bool import_ok = false; std::string written_file;
	ResourceType type = GetTypeFromPath(new_file_in_assets);
	std::string copy_path;

	switch (type) 
	{
	case texture:
		//App->filesystem->CopyFileTo(new_file_in_assets, "Assets\\Meshes", &copy_path);
		App->material_importer->ImportImage(new_file_in_assets); 
		break;
	case mesh: 
		//import_ok = App->mesh_importer->ImportMesh(new_file_in_assets, "", written_file); 
		break;
	case scene: 
		//import_ok = App->scene_importer->SaveScene(new_file_in_assets, written_file); 
		break;
	}

	//if (import_ok == true) // If export was successful, create a new resource
	//{ 
	//	Resource* res = CreateNewResource(type);
	//	res->file = new_file_in_assets;
	//	res->exported_file = written_file;
	//	ret = res->uid;
	//}
	return ret;

}

Resource * ModuleResources::CreateNewResource(ResourceType type, uint default_id)
{
//	Resource* ret = nullptr;
//	uint uid = GenerateNewUID();
//
//	switch (type)
//	{
//	case texture: 
//		ret = (Resource*) new ResourceTexture(uid); break;
//
//	case mesh: 
//		ret = (Resource*) new ResourceMesh(uid); break;	
//
//	case scene: 
//		ret = (Resource*) new ResourceScene(uid); break;
//	}
//
//	if (ret != nullptr)
//		resources[uid] = ret;
//
//	return ret;
//}
//
//Resource * ModuleResources::Get(uint uid)
//{
//	std::map<uint, Resource*>::iterator it = resources.find(uid);
//	if (it != resources.end())
//		return it->second;
	return nullptr;
}

uint ModuleResources::CreateNewResourceInPath(const char* path)
{
	uint uid_key = 0;

	FILE* f = fopen(path, "rb");
	if (f != nullptr)
	{
		fseek(f, 0L, SEEK_END);
		uint total_size = ftell(f);
		rewind(f); //go back to file begining

		char* data = new char[total_size];
		memccpy(data, f, sizeof(char), total_size);
		fclose(f);

		uid_key = pcg32_random_r(&App->rng);
		RELEASE_ARRAY(data);
	}

	return uid_key;
}

std::string ModuleResources::GetNameFromPath(const char * path)
{
	std::string _path = path;
	std::string name;

	uint dot = _path.find_last_of(".");
	uint slash = _path.find_last_of("\\");
	slash += 1;
	name = _path.substr(slash, dot - slash);

	return name;
}

std::string ModuleResources::GetExtension(const char* path)
{
	std::string _path = path;
	std::string ext;

	uint dot = _path.find_last_of(".");
	ext = _path.substr(dot + 1);

	return ext;
}

std::string ModuleResources::GetFilePath(const char* path)
{
	std::string _path = path;
	std::string rel_path;

	uint slash = _path.find_last_of("\\");
	rel_path = _path.substr(0, slash + 1);

	return rel_path;
}

ResourceType ModuleResources::GetTypeFromPath(const char * path) const
{
	ResourceType ret = unknown;

	std::string p = path;
	uint cut = p.find_last_of(".");
	std::string extension = p.substr(cut + 1);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	if (extension == "json") // meta, scenes and prefabs.
	{
		int type_cut = p.find_last_of("_");
		if (type_cut != -1)
		{
			std::string type_str = p.substr(type_cut + 1, cut - (type_cut + 1));
			if (type_str == "scene")
				ret = scene;
		}
		else
			CONSOLELOG("The file %s is not supported.", path);
	}
	else if (extension == "fbx") // meshes
	{
		ret = mesh;
	}
	else if (extension == "png" || extension == "dds") // textures
	{
		ret = texture;
	}

	return ret;
}

Resource * ModuleResources::ExistResource(std::string & file, int cn)
{
	Resource* ret = nullptr;

	std::string file_name = GetNameFromPath(file.c_str());

	for (std::map<uint, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		for (std::vector<Resource*>::iterator r = it->second.begin(); r != it->second.end(); ++r)
		{
			if ((*r)->name == file_name)
			{
				if (cn != -1)
				{
					if ((*r)->comp_number == cn)
					{
						ret = (*r);
						break;
					}
				}
				else
				{
					ret = (*r);
					break;
				}
			}
		}
	}

	return ret;
}

void ModuleResources::CreateMeta(const char * path)
{
	/*uint key = CreateResource(path);
	std::string lib_path = GetRelativePathFromPath(path);
	lib_path += GetNameFromPath(path);
	lib_path += "_meta.json";
	JSON_Doc* meta = App->json->CreateJSON(lib_path.c_str());

	meta->AddArray("resources");
	for (int i = 0; i < resources[key].size(); ++i)
	{
		meta->AddSectionToArray("resources");
		meta->MoveToSectionFromArray("resources", meta->GetArraySize("resources") - 1);

		meta->SetString("name", resources[key][i]->name.c_str());
		meta->SetString("extension", resources[key][i]->extension.c_str());
		meta->SetString("rel_path", resources[key][i]->rel_path.c_str());
		meta->SetString("lib_name", resources[key][i]->lib_name.c_str());
		meta->SetString("lib_path", resources[key][i]->lib_path.c_str());
		meta->SetNumber("file_id", resources[key][i]->file_id);
		meta->SetNumber("type", resources[key][i]->type);
		meta->SetNumber("UID", resources[key][i]->UID);
		meta->SetBool("flipped", resources[key][i]->flipped);
		meta->SetNumber("sh_type", resources[key][i]->sh_type);

		meta->MoveToFirstObject();*/
	//}

	//meta->SaveFile();
}


