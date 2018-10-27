#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "PanelProperties.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/lib/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
	/*if (debug_draw == true)
	{
		BeginDebugDraw();
		App->DebugDraw();
		EndDebugDraw();
	}*/


	App->scene_intro->Draw();
	Draw();
	App->editor->Draw();

	App->camera->GetSceneTexture()->Bind();

	SDL_GL_SwapWindow(App->window->window);
	
	return UPDATE_CONTINUE;
}

// Called before quitting
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

//----------------------------------------------------------------

void ModuleRenderer3D::Draw()
{
	for (int i = 0; i < comp_meshes.size(); i++)
	{
		//glEnable(GL_TEXTURE_2D);

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, comp_meshes[i]->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp_meshes[i]->id_indices);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, comp_meshes[i]->id_uvs);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

		//glBindTexture(GL_TEXTURE_2D, (GLuint)tex.id_texture);
		glDrawElements(GL_TRIANGLES, comp_meshes[i]->num_indices, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		//glDisable(GL_TEXTURE_2D);
	}
}

void ModuleRenderer3D::LoadGeometry(const char* path, GameObject* game_obj)
{
	DeleteGeometry();
	App->material->DeleteTextures();

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	int totalvertices = 0;
	int totalindices = 0;

	if (scene != nullptr && scene->HasMeshes())
	{
		GetTransformation(scene->mRootNode);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			ComponentMesh* m = CreateComponentMesh();
			game_obj->AddComponent(m);
			aiMesh* new_mesh = scene->mMeshes[i];

			// copy vertices
			m->num_vertices = new_mesh->mNumVertices;
			m->vertices = new float[m->num_vertices * 3];
			memcpy(m->vertices, new_mesh->mVertices, sizeof(float) * m->num_vertices * 3);

			totalvertices += m->num_vertices;
			CONSOLELOG("New mesh with %d vertices", m->num_vertices);

			// load buffer for vertices
			glGenBuffers(1, (GLuint*) &(m->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, m->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertices * 3, m->vertices, GL_STATIC_DRAW);

			// copy indices
			if (new_mesh->HasFaces())
			{
				m->num_indices = new_mesh->mNumFaces * 3;
				totalindices += m->num_indices;
				m->indices = new uint[m->num_indices]; // assume each face is a triangle

				for (uint i = 0; i < new_mesh->mNumFaces; i++)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
					{
						CONSOLELOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m->indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				// load buffer for indices
				glGenBuffers(1, (GLuint*) &(m->id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m->num_indices, m->indices, GL_STATIC_DRAW);
			}

			// copy uvs
			if (new_mesh->HasTextureCoords(0))
			{
				m->num_uvs = new_mesh->mNumVertices;
				m->texture_coords = new float[m->num_uvs * 3];
				memcpy(m->texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * m->num_uvs * 3);

				// load buffer for uvs
				glGenBuffers(1, (GLuint*) &(m->id_uvs));
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint)m->id_uvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * m->num_uvs * 3, m->texture_coords, GL_STATIC_DRAW);
			}

			AABB box;
			box.SetNegativeInfinity();
			box.Enclose((float3*)new_mesh->mVertices, new_mesh->mNumVertices);

			vec3 midpoint = (box.CenterPoint().x, box.CenterPoint().y, box.CenterPoint().z);
			App->camera->Position = midpoint + (App->camera->Z *  box.Size().Length() * 1.2f);

		}

		App->editor->properties->SaveMeshInfo(path, scene->mNumMeshes, totalvertices, (totalindices / 3));
		aiReleaseImport(scene);
	}
	else
		CONSOLELOG("Error loading scene %s", path);
}

void ModuleRenderer3D::DeleteGeometry()
{
	if (comp_meshes.size() > 0) {
		while (comp_meshes.size() > 0)
		{
			glDeleteBuffers(1, (GLuint*)comp_meshes[comp_meshes.size() - 1]->id_uvs);
			glDeleteBuffers(1, (GLuint*)comp_meshes[comp_meshes.size() - 1]->id_indices);
			glDeleteBuffers(1, (GLuint*)comp_meshes[comp_meshes.size() - 1]->id_vertices);

			comp_meshes.pop_back();
		}

		App->editor->properties->EraseGeometryInfo();

		CONSOLELOG("Old geometry deleted");
	}
}

void ModuleRenderer3D::GetTransformation(aiNode* scene)
{
	aiVector3D pos, scale;
	aiQuaternion rot;

	aiMatrix4x4 matrix = scene->mTransformation;

	if (scene->mNumChildren > 0)
	{
		for (int i = 0; i < scene->mNumChildren; i++)
		{
			GetTransformation(scene->mChildren[i]);
			matrix *= scene->mChildren[i]->mTransformation;
		}
	}

	matrix.Decompose(scale, rot, pos);

	App->editor->properties->SaveTransformationInfo(pos, rot, scale);
}

ComponentMesh* ModuleRenderer3D::CreateComponentMesh()
{
	ComponentMesh* new_comp = new ComponentMesh;
	comp_meshes.push_back(new_comp);

	return new_comp;
}


