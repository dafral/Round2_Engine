#ifndef __SCENEIMPORTER_H__
#define __SCENEIMPORTER_H__

#include "Module.h"

class SceneImporter : public Module
{
public:
	SceneImporter(Application* app, bool start_enabled = true);
	~SceneImporter();

	void SaveScene(const char* path);

};

#endif
