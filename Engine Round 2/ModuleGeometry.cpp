#include "Globals.h"
#include "Application.h"
#include "ModuleGeometry.h"
#include "PanelProperties.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Devil/include/IL/il.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

//Constructor
ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleGeometry::~ModuleGeometry()
{}

bool ModuleGeometry::Start()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	// DeviL Initialization
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

bool ModuleGeometry::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleGeometry::LoadGeometry(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	int totalvertices = 0;

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh m;	
			aiMesh* new_mesh = scene->mMeshes[i];

			// copy vertices
			m.num_vertices = new_mesh->mNumVertices;
			m.vertices = new float[m.num_vertices * 3];
			memcpy(m.vertices, new_mesh->mVertices, sizeof(float) * m.num_vertices * 3);

			totalvertices += m.num_vertices;
			CONSOLELOG("New mesh with %d vertices", m.num_vertices);

			// load buffer for vertices
			glGenBuffers(1, (GLuint*) &(m.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, m.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertices * 3, m.vertices, GL_STATIC_DRAW);

			// copy indices
			if (new_mesh->HasFaces())
			{
				m.num_indices = new_mesh->mNumFaces * 3;
				m.indices = new uint[m.num_indices]; // assume each face is a triangle

				for (uint i = 0; i < new_mesh->mNumFaces; i++)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
					{
						CONSOLELOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				// load buffer for indices
				glGenBuffers(1, (GLuint*) &(m.id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m.num_indices, m.indices, GL_STATIC_DRAW);
			}

			// copy uvs
			if (new_mesh->HasTextureCoords(0))
			{
				m.num_uvs = new_mesh->mNumVertices;
				m.texture_coords = new float[m.num_uvs * 3];
				memcpy(m.texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * m.num_uvs * 3);

				// load buffer for uvs
				glGenBuffers(1, (GLuint*) &(m.id_uvs));
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint)m.id_uvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * m.num_uvs * 3, m.texture_coords, GL_STATIC_DRAW);
			}

			meshes.push_back(m);
		}

		App->editor->properties->SaveMeshInfo(path, scene->mNumMeshes, totalvertices);
		aiReleaseImport(scene);
	}
	else
		CONSOLELOG("Error loading scene %s", path);
}

void ModuleGeometry::LoadTexture(const char* full_path)
{
	ILuint imageID;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(full_path))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		//Flip the image if it is upside-down
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}

		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			error = ilGetError();
			CONSOLELOG("Texture conversion failed: %d %s", error, iluErrorString(error));
		}
		else
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &tex.id_texture);
			glBindTexture(GL_TEXTURE_2D, tex.id_texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//Texture Specifications
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		}
	}
	else {
		error = ilGetError();
	}

	ilDeleteImages(1, &imageID);
}

void ModuleGeometry::Draw()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		glEnable(GL_TEXTURE_2D);

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].id_indices);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].id_uvs);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

		glBindTexture(GL_TEXTURE_2D, (GLuint)tex.id_texture);
		glDrawElements(GL_TRIANGLES, meshes[i].num_indices, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisable(GL_TEXTURE_2D);
	}
}