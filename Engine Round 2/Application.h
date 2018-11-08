#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "MaterialImporter.h"
#include "MeshImporter.h"
#include "SceneImporter.h"
#include "ModuleFileSystem.h"
#include "ModuleJSON.h"
#include "PanelConsole.h"
#include "PanelConfiguration.h"

#include "ImGui/imgui.h"
#include "PCG/pcg_basic.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	MaterialImporter* material_importer;
	MeshImporter* mesh_importer;
	SceneImporter* scene_importer;
	ModuleFileSystem* filesystem;
	ModuleJSON* json;

	ImGuiTextBuffer consoleText; 
	pcg32_random_t rng;

private:
	
	Timer	ms_timer;
	Timer	fps_timer;
	float	dt;
	float   fps_counter;
	int     last_frame_fps;
	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;