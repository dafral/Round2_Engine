#pragma once
#include "Module.h"
#include "Globals.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct Texture
{
	uint id_texture = 0;
};

struct Mesh
{
	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_uvs = 0; // id in VRAM
	uint num_uvs = 0;
	float* texture_coords = nullptr;
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
	void LoadTexture(const char* path);
	void DeleteGeometry();
	void DeleteTextures();
	void GetTransformation(aiNode* scenenode);
	void Draw();

private:
	std::vector<Mesh> meshes;
	Texture tex;
};