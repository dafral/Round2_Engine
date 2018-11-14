#include "Application.h"
#include "ModuleResources.h"

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

	//switch (type) 
	//{
	//case texture:
	//	import_ok = App->material_importer->Import(); break;
	//case mesh: 
	//	import_ok = App->mesh_importer->ImportMesh(new_file_in_assets, "", written_file); break;

	//case scene: 
	//	import_ok = App->scene_importer->SaveScene(new_file_in_assets, written_file); break;
	//}

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