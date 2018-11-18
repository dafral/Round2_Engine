#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "Component_Camera.h"
#include "GameObject.h"
#include "TextureMSAA.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleRenderer3D::~ModuleRenderer3D()
{}

bool ModuleRenderer3D::Init()
{
	CONSOLELOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		CONSOLELOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		GLenum err = glewInit();
		if (err != GL_NO_ERROR)
		{
			CONSOLELOG("Glew library could not be initialized! Error: %s\n", gluErrorString(err));
			ret = false;
		}

		else
		{
			CONSOLELOG("Using Glew %s", glewGetString(GLEW_VERSION));
			CONSOLELOG("Vendor: %s", glGetString(GL_VENDOR));
			CONSOLELOG("Renderer: %s", glGetString(GL_RENDERER));
			CONSOLELOG("OpenGL version supported %s", glGetString(GL_VERSION));
			CONSOLELOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

			ret = true;
		}
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			CONSOLELOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLELOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLELOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Initialize Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLELOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glEnable(GL_DITHER);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	// =============================================================
	// Game camera 
	// =============================================================

	App->camera->GetGameTexture()->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->GetGameCamera()->GetProjectionMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetGameCamera()->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->GetGameCamera()->GetPosition().x, App->camera->GetGameCamera()->GetPosition().y, App->camera->GetGameCamera()->GetPosition().z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	App->scene->Draw();
	DrawGO(false);

	if (App->camera->mouse_ray.Length() > 0)
	{
		glBegin(GL_LINES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(10.0f);
		glDisable(GL_CULL_FACE);
		glColor3f(255, 0, 255);

		glVertex3fv((GLfloat*)&App->camera->mouse_ray.a);
		glVertex3fv((GLfloat*)&App->camera->mouse_ray.b);

		glEnd();
		glLineWidth(1.0f);
		glColor3f(200, 0, 125);
		glEnable(GL_CULL_FACE);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	// =============================================================
	// Scene camera 
	// =============================================================

	App->camera->GetSceneTexture()->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->GetSceneCamera()->GetProjectionMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetSceneCamera()->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->GetSceneCamera()->GetPosition().x, App->camera->GetSceneCamera()->GetPosition().y, App->camera->GetSceneCamera()->GetPosition().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	App->scene->Draw();
	DrawGO(true);

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	App->editor->Draw();

	SDL_GL_SwapWindow(App->window->window);
	
	return UPDATE_CONTINUE;
}

bool ModuleRenderer3D::CleanUp()
{
	CONSOLELOG("Destroying 3D Renderer");
	
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetVsync(bool vsync)
{
	SDL_GL_SetSwapInterval(vsync);
}

// =====================================================================
// Component mesh
// =====================================================================

Component_Mesh* ModuleRenderer3D::CreateComponentMesh(GameObject* my_go)
{
	Component_Mesh* cmesh = new Component_Mesh();
	cmesh->my_go = my_go;
	my_go->AddComponent(cmesh);
	meshes.push_back(cmesh);	
	return cmesh;
}

std::vector<Component_Mesh*>* ModuleRenderer3D::GetMeshesVector()
{
	return &meshes;
}

Component_Mesh* ModuleRenderer3D::IsMeshLoaded(Component_Mesh* new_mesh)
{
	Component_Mesh* ret = nullptr;
	bool is_loaded = true;

	for (int i = 0; i < meshes.size(); i++)
	{
		// Vertices
		if (meshes[i]->GetNumVertices() == new_mesh->GetNumVertices())
		{
			for (unsigned int j = 0; j < meshes[i]->GetNumVertices() * 3; j++)
				if (meshes[i]->GetVertices()[j] != new_mesh->GetVertices()[j])
					is_loaded = false;
		}
		else is_loaded = false;

		// Indices
		if (meshes[i]->GetNumIndices() == new_mesh->GetNumIndices())
		{
			for (unsigned int j = 0; j < meshes[i]->GetNumIndices(); j++)
				if (meshes[i]->GetIndices()[j] != new_mesh->GetIndices()[j])
					is_loaded = false;
		}
		else is_loaded = false;

		if (is_loaded == true)
		{
			ret = meshes[i];
			break;
		}
	}

	return ret;
}

// ==================================================================================
// Draw
// ==================================================================================

void ModuleRenderer3D::AddGOToDraw(GameObject* go, Component_Camera* curr_camera)
{
	if (curr_camera == App->camera->GetSceneCamera())
		go_scene.push_back(go);
	else
		go_game.push_back(go);
}

void ModuleRenderer3D::DrawGO(bool is_editor_cam)
{
	if (is_editor_cam)
	{
		std::sort(go_scene.begin(), go_scene.end());
		go_scene.erase(std::unique(go_scene.begin(), go_scene.end()), go_scene.end());

		for (int i = 0; i < go_scene.size(); i++)
		{
			if(go_scene[i]->GetVisible()) go_scene[i]->Draw(true);
		}
		go_scene.clear();
	}
	else
	{
		std::sort(go_game.begin(), go_game.end());
		go_game.erase(std::unique(go_game.begin(), go_game.end()), go_game.end());

		for (int i = 0; i < go_game.size(); i++)
		{
			if (go_game[i]->GetVisible()) go_game[i]->Draw(false);
		}
		go_game.clear();
	}
}


