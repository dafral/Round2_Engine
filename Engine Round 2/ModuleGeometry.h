#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


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