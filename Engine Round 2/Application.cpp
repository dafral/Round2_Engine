#include "Application.h"
#include <time.h>

Application::Application()
{
	debug = new DebugDraw();

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	mesh_importer = new MeshImporter(this);
	material_importer = new MaterialImporter(this);
	filesystem = new ModuleFileSystem(this);
	json = new ModuleJSON(this);
	scene_importer = new SceneImporter(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(filesystem);
	AddModule(json);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene);
	AddModule(scene_importer);

	// Renderer last!
	AddModule(renderer3D);
	AddModule(mesh_importer);
	AddModule(material_importer);
	AddModule(editor);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}

	delete debug;
}

bool Application::Init()
{
	bool ret = true;
	fps_timer.Start();

	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 1000);

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled()) ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	CONSOLELOG("-------------- Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled()) ret = item->data->Start();
		item = item->next;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	fps_counter++;

	if (fps_timer.Read() >= 1000) // 1 sec
	{
		last_frame_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	// FPS cap
	int frame_ms = (1000 / App->editor->configuration->GetFPSCap()) - ms_timer.Read();
	if (frame_ms > 0) SDL_Delay(frame_ms);

	App->editor->configuration->AddFps(last_frame_fps);
	App->editor->configuration->AddMs(ms_timer.Read());
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled()) ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}