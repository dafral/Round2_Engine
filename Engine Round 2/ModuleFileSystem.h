#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"
#include <string>

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();


	std::string CreateFolder(const char* path, const char* name);


};

#endif // __FILESYSTEM_H__
