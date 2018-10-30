#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"
#include <string>

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	//std::string CreateFolder(const char* path, const char* name);


};

#endif // __FILESYSTEM_H__
