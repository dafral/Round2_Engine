#include "Globals.h"
#include "ModuleFileSystem.h"
#include <Windows.h>

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

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
		CONSOLELOG("Error creating folder (Folder aleady exists): %s", filepath.c_str())
	}

	ret = filepath + '\\';

	return ret;
}
