#ifndef __MODULERESOURCES_H__
#define __MODULERESOURCES_H__

#include "Module.h"
#include "Resource.h"
#include <map>

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	//uint Find(const char* file_in_assets) const;
	uint ImportFile(const char* new_file_in_assets);
	//uint GenerateNewUID();
	//const Resource* Get(uint uid) const;
	//Resource* Get(uint uid);
	Resource* CreateNewResource(ResourceType type, uint force_uid = 0);
private:
	uint last_uid = 1;
	std::map<uint, Resource*> resources;
};

#endif // !__MODULERESOURCES_H__

