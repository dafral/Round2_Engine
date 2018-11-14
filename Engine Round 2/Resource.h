#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"
#include <string>

enum ResourceType
{
	texture,
	mesh,
	scene, 
	unknown
};

class Resource
{
public:
	//Resource(uint uid, ResourceType type);
	//virtual ~Resource();
	//Resource::ResourceType GetType() const;
	//uint GetUID() const;
	//const char* GetFile() const;
	//const char* GetExportedFile() const;
	//bool IsLoadedToMemory() const;
	//bool LoadToMemory();
	//uint CountReferences() const;
	/*virtual void Save(Config& config) const;
	virtual void Load(const Config& config);*/
	virtual bool LoadInMemory() = 0;public:

	uint uid = 0;
	std::string file;
	std::string exported_file;
	ResourceType type = unknown;
	uint loaded = 0;

};
#endif // !__RESOURCE_H__

