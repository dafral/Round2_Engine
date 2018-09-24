#pragma once
#include "Module.h"
#include "Globals.h"

struct Mesh
{
	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
};


class ModuleGeometry : public Module
{
public:
	ModuleGeometry(Application* app, bool start_enabled = true);
	~ModuleGeometry();

	bool Start();
	/*update_status Update(float dt);*/
	bool CleanUp();

	void LoadGeometry(const char* path);
	void Draw();

	//char* path;
	std::vector<Mesh> meshes;
};