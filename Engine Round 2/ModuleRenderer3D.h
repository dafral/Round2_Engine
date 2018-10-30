#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include <vector>

#define MAX_LIGHTS 8

class GameObject;
class Component_Mesh;
struct aiMesh;
struct aiScene;
struct aiNode;

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
	
	Component_Mesh* CreateComponentMesh(GameObject* my_go);
	void LoadScene(char* full_path);
	void LoadMesh(GameObject* parent, const aiScene* scene, aiNode* node);
	void LoadBuffers(Component_Mesh* cmesh, aiMesh* new_mesh);
	void DrawMeshes();
	Component_Mesh* IsMeshLoaded(const Component_Mesh* curr_mesh);
	std::vector<Component_Mesh*>* GetMeshesVector();

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	float color_hue[3] = { 1.0f, 1.0f, 1.0f };

private:
	std::vector<Component_Mesh*> meshes;
};