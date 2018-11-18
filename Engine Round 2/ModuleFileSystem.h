#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"
#include <string>
#include <vector>

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool CleanUp();

	const char* GetRootPath();
	std::string CreateFolder(const char* path, const char* name);
	bool SaveFile(const char* path, const char* file_content, const char* name, const char* extension, int size);
	void DeleteFilesInPath(const char* path);

	void CopyFileTo(const char * file, const char * target);
	void GetFilesInPath(std::vector<std::string>& paths, const char * path, const char * extension = nullptr);

	std::string GetExtension(const char* path);
	std::string GetNameWithoutPath(const char* path, bool delete_extension);


	std::string assets_path;
	std::string scene_path;
	std::string library_path;
	std::string library_mesh_path;
	std::string library_material_path;

};

#endif // __FILESYSTEM_H__
