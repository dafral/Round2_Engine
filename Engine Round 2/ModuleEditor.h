#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
//#include "ImGui\imgui.h"


class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void Draw();



	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;
	bool blend = true;
	bool dither = true;
	bool wireframe = false;
};

#endif // __ModuleEditor_H__
