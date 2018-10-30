#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"
#include <string>

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	const char* GetRootPath();
	std::string CreateFolder(const char* path, const char* name);
	bool SaveFile(const char * path, const char * file_content, const char * name, const char * extension, int size);

	std::string assets_path;
	std::string library_path;
	std::string library_mesh_path;
	std::string library_material_path;

};

#endif // __FILESYSTEM_H__
