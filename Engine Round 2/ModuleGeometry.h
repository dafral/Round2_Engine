#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include <vector>

struct Texture
{
	uint id_texture = 0;
};

class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Start();
	/*update_status Update(float dt);*/
	bool CleanUp();

	void LoadTexture(const char* path);
	void DeleteTextures();

	void Draw();

private:

	Texture tex;
};