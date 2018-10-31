#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "PanelProperties.h"
#include "ModuleScene.h"
#include "Component_Mesh.h"
#include "GameObject.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	mesh_importer = new MeshImporter;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	App->scene->Draw();
	int prueba = App->renderer3D->meshes.size();
	DrawMeshes();//Draw of all meshes
	App->editor->Draw();

	App->camera->GetSceneTexture()->Bind();
	SDL_GL_SwapWindow(App->window->window);
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	CONSOLELOG("Destroying 3D Renderer");
	
	RELEASE(mesh_importer);
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
	meshes.push_back(cmesh);	
	return cmesh;
}

// =====================================================================
// Loading Scene
// =====================================================================

void ModuleRenderer3D::LoadScene(char* full_path)
{
	// Creating the parent (empty game object) 
	GameObject* empty_go = App->scene->CreateGameObject("test", App->scene->root_node);

	// Loading scene
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	aiNode* node = scene->mRootNode;

	if (scene != nullptr && scene->HasMeshes())
	{
		LoadMesh(empty_go, scene, node);
		CONSOLELOG("Scene %s loaded.", full_path);
	}
	else
	{
		CONSOLELOG("Error loading scene %s.", full_path);
	}

	// Releasing scene
	aiReleaseImport(scene);
}

void ModuleRenderer3D::LoadMesh(GameObject* parent, const aiScene* scene, aiNode* node)
{
	if (node->mNumMeshes <= 0)
	{
		// Recursion
		for (int i = 0; i < node->mNumChildren; i++)
			LoadMesh(parent, scene, node->mChildren[i]);
	}
	else
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[i]];

			// Creating a Game Object (child of parent) for each mesh.
			GameObject* go = App->scene->CreateGameObject(node->mName.C_Str(), parent);
			Component_Mesh* cmesh = CreateComponentMesh(go);

			// Vertices
			cmesh->num_vertices = new_mesh->mNumVertices;
			cmesh->vertices = new float[cmesh->num_vertices * 3];
			memcpy(cmesh->vertices, new_mesh->mVertices, sizeof(float) * cmesh->num_vertices * 3);

			// Indices
			if (new_mesh->HasFaces())
			{
				cmesh->num_indices = new_mesh->mNumFaces * 3;
				cmesh->indices = new uint[cmesh->num_indices];
				for (uint i = 0; i < new_mesh->mNumFaces; i++)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) 
					{
						CONSOLELOG("WARNING, geometry face with != 3 indices!");
					}
					else 
					{
						memcpy(&cmesh->indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			// UVs
			if (new_mesh->HasTextureCoords(0))
			{
				cmesh->num_uvs = new_mesh->mNumVertices;
				cmesh->texture_coords = new float[cmesh->num_uvs * 3];
				memcpy(cmesh->texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * cmesh->num_uvs * 3);
			}

			// Check if we already loaded this mesh in memory
			Component_Mesh* aux = nullptr;
			if (aux == nullptr)
			{
				LoadBuffers(cmesh, new_mesh);
			}
			else
			{
				cmesh->id_indices = aux->id_indices;
				cmesh->id_vertices = aux->id_vertices;
				cmesh->id_uvs = aux->id_uvs;
			}

			// Import mesh HERE
			mesh_importer->Save(App->filesystem->library_mesh_path.c_str(), cmesh);

			// Recursion
			for (int i = 0; i < node->mNumChildren; i++)
				LoadMesh(go, scene, node->mChildren[i]);
		}
	}
}

Component_Mesh* ModuleRenderer3D::IsMeshLoaded(const Component_Mesh* new_component)
{
	Component_Mesh* ret = nullptr;

	for (int i = 0; i < meshes.size(); i++)
	{
		bool loaded = true;

		// Vertices
		if (meshes[i]->num_vertices == new_component->num_vertices)
		{
			for (unsigned int j = 0; j < meshes[i]->num_vertices * 3; j++)
				if (meshes[i]->vertices[j] != new_component->vertices[j])
					loaded = false;
		}
		else loaded = false;

		// Indices
		if (meshes[i]->num_indices == new_component->num_indices)
		{
			for (unsigned int j = 0; j < meshes[i]->num_indices; j++)
				if (meshes[i]->indices[j] != new_component->indices[j])
					loaded = false;
		}
		else loaded = false;

		if (loaded == true)
		{
			ret = meshes[i];
			break;
		}
	}

	return ret;
}

void ModuleRenderer3D::LoadBuffers(Component_Mesh* cmesh, aiMesh* new_mesh)
{
	// Load buffer for vertices
	glGenBuffers(1, (GLuint*) &(cmesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, cmesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cmesh->num_vertices * 3, cmesh->vertices, GL_STATIC_DRAW);

	// Load buffer for indices
	if (new_mesh->HasFaces())
	{
		glGenBuffers(1, (GLuint*) &(cmesh->id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * cmesh->num_indices, cmesh->indices, GL_STATIC_DRAW);
	}

	// Load buffer for UVs
	if (new_mesh->HasTextureCoords(0))
	{
		glGenBuffers(1, (GLuint*) &(cmesh->id_uvs));
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)cmesh->id_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * cmesh->num_uvs * 3, cmesh->texture_coords, GL_STATIC_DRAW);
	}
}

void ModuleRenderer3D::DrawMeshes()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->id_indices);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->id_uvs);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, meshes[i]->num_indices, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

std::vector<Component_Mesh*>* ModuleRenderer3D::GetMeshesVector()
{
	return &meshes;
}
