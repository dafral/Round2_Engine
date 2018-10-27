#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ModuleMaterial.h"
#include "Component.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#define MAX_LIGHTS 8

class Component;
class ComponentMesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void SetVsync(bool vsync);

	void LoadGeometry(const char* path);
	void DeleteGeometry();
	void GetTransformation(aiNode* scenenode);
	//ComponentMesh* CreateComponentMesh();

	void Draw();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	float color_hue[3] = { 1.0f, 1.0f, 1.0f };
	std::vector<ComponentMesh*> comp_meshes;
};