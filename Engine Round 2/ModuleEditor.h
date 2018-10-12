#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include <vector>

class Panel;
class PanelConfiguration;
class PanelProperties;
class PanelConsole;
class PanelScene;

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

public:

	PanelConfiguration* configuration = nullptr;
	PanelProperties* properties = nullptr;
	PanelConsole* console = nullptr;
	PanelScene* scene = nullptr;

	std::vector<Panel*> panels;
};

#endif // __ModuleEditor_H__
