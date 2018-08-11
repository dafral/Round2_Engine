#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
//#include "ImGui\imgui.h"


class ModuleEditor : public Module
{
public:

	ModuleEditor (Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();


};

#endif // __ModuleEditor_H__
