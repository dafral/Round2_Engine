#pragma once
#include "Module.h"
#include "Globals.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "glmath.h"
#include "Light.h"

#include <vector>
#include <array>

#define MAX_LIGHTS 8

class GameObject;
class Component_Mesh;
class Component_Camera;

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
	Component_Mesh* IsMeshLoaded(Component_Mesh* curr_mesh);
	std::vector<Component_Mesh*>* GetMeshesVector();

	void AddGOToDraw(GameObject* go, Component_Camera* curr_camera);
	void DrawGO(bool is_editor_cam);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	float color_hue[3] = { 1.0f, 1.0f, 1.0f };

private:
	std::vector<GameObject*> go_scene;
	std::vector<GameObject*> go_game;
	std::vector<Component_Mesh*> meshes;
};