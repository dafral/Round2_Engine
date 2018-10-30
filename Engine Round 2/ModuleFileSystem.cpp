#include "Globals.h"
#include "ModuleFileSystem.h"
#include <Windows.h>

#include "SDL/include/SDL_filesystem.h"

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	assets_path = CreateFolder(GetRootPath(), "Assets");
	library_path = CreateFolder(GetRootPath(), "Library");
	library_mesh_path = CreateFolder(library_path.c_str(), "Meshes");
	library_material_path = CreateFolder(library_path.c_str(), "Textures");

}

ModuleFileSystem::~ModuleFileSystem()
{}

std::string ModuleFileSystem::CreateFolder(const char * path, const char * name)
{
	std::string ret;

	std::string filepath = path;

	if (filepath[filepath.length() - 1] != '\\')
	{
		filepath += '\\';
	}

	filepath += name;

	DWORD error = GetLastError();

	if (CreateDirectory(filepath.c_str(), NULL) == 0)
	{
		error = GetLastError();
	}

	if (error == ERROR_PATH_NOT_FOUND)
	{
		CONSOLELOG("Error creating folder (path not found): %s", path);
		return ret;
	}
	else if (error == ERROR_ALREADY_EXISTS)
	{
		CONSOLELOG("Error creating folder (Folder aleady exists): %s", filepath.c_str());
	}

	ret = filepath + '\\';

	CONSOLELOG("Folder %s created!", filepath.c_str());

	return ret;
}

const char* ModuleFileSystem::GetRootPath()
{
	return SDL_GetBasePath();
}
