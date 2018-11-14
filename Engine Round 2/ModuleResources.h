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

	uint ImportFile(const char* new_file_in_assets);
	Resource* CreateNewResource(ResourceType type, uint force_uid = 0);
	uint CreateNewResourceInPath(const char* path);

	std::string GetNameFromPath(const char * path);
	ResourceType GetTypeFromPath(const char * path) const;
	std::string GetExtension(const char* path);
	std::string GetFilePath(const char* path);

	Resource * ExistResource(std::string & file, int file_id = -1);
public:
	uint last_uid = 1;
	std::map<uint, std::vector<Resource*>> resources;

};

#endif // !__MODULERESOURCES_H__

