#include "Globals.h"
#include "ModuleFileSystem.h"
#include <Windows.h>

#include "SDL/include/SDL_filesystem.h"

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	assets_path = CreateFolder(GetRootPath(), "Assets");
	scene_path = assets_path += "scene.json";
	library_path = CreateFolder(GetRootPath(), "Library");
	library_mesh_path = CreateFolder(library_path.c_str(), "Meshes");
	library_material_path = CreateFolder(library_path.c_str(), "Materials");

}

ModuleFileSystem::~ModuleFileSystem()
{}

bool ModuleFileSystem::CleanUp()
{	
	return true;
}

std::string ModuleFileSystem::CreateFolder(const char* path, const char * name)
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

bool ModuleFileSystem::SaveFile(const char* path, const char* file_content, const char* name, const char* extension, int size)
{
	bool ret = false;

	std::string file = path;
	file += name;
	file += ".";
	file += extension;

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		CONSOLELOG("File %s saved!", name);
		ret = true;
	}
	else
	{
		CONSOLELOG("Error saving file %s: ", name);
	}

	fclose(new_file);

	return ret;
}

void ModuleFileSystem::CopyFileTo(const char * file, const char * target)
{
	std::string curr_file = file;
	uint cut = curr_file.find_last_of("\\");
	std::string dest_file = target;

	if (dest_file.find_last_of("\\") == dest_file.size() - 1)
		dest_file += curr_file.substr(cut + 1, curr_file.size() - cut + 1);
	
	else
		dest_file += curr_file.substr(cut, curr_file.size() - cut);

	CopyFile(file, dest_file.c_str(), false);
}


void ModuleFileSystem::DeleteFilesInPath(const char* path)
{
	if (DeleteFile(path) == 0)
	{
		DWORD error = GetLastError();

		if (error == ERROR_FILE_NOT_FOUND)
		{
			CONSOLELOG("Error deleting file (path not found)): %s", path);
		}
	}
}

void ModuleFileSystem::GetFilesInPath(std::vector<std::string>& paths, const char * path, const char * extension)
{
	WIN32_FIND_DATA search_data;

	std::string path_ex = path;

	if (extension != nullptr)
	{
		path_ex += "*.";
		path_ex += extension;
	}
	else
	{
		path_ex += "*.*";
	}

	HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		std::string path_new = path;
		path_new += search_data.cFileName;
		paths.push_back(path_new);

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	if (handle)
		FindClose(handle);
}

std::string ModuleFileSystem::GetExtension(const char* path)
{
	std::string _path = path;
	std::string ext;

	uint dot = _path.find_last_of(".");
	ext = _path.substr(dot + 1);

	return ext;
}

std::string ModuleFileSystem::GetNameWithoutPath(const char* path, bool delete_extension)
{
	std::string filename = path;
	const size_t last_slash_idx = filename.find_last_of("\\/");

	if (std::string::npos != last_slash_idx)
	{
		filename.erase(0, last_slash_idx + 1);
	}

	// Remove extension if present.
	if (delete_extension)
	{
		const size_t period_idx = filename.rfind('.');
		if (std::string::npos != period_idx)
		{
			filename.erase(period_idx);
		}
	}

	return filename;
}

std::string ModuleFileSystem::GetDirectory(const char* path)
{
	char sep = '/';
	std::string s = path;

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(0, i + 1));
	}

	return("");
}
