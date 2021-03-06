#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include <vector>

class Panel;
class PanelConfiguration;
class PanelProperties;
class PanelConsole;
class PanelScene;
class PanelHierarchy;
class PanelGame;
class PanelTime;

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
	PanelHierarchy* hierarchy = nullptr;
	PanelGame* game = nullptr;
	PanelTime* time = nullptr;

	std::vector<Panel*> panels;
};

#endif // __ModuleEditor_H__
