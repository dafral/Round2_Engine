#include "Globals.h"
#include "Application.h"
#include "ModuleMaterial.h"
#include "PanelProperties.h"
#include "MathGeoLib/MathGeoLib.h"

#include "Devil/include/IL/il.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

//Constructor
ModuleMaterial::ModuleMaterial(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleMaterial::~ModuleMaterial()
{}

bool ModuleMaterial::Start()
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

bool ModuleMaterial::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}


void ModuleMaterial::LoadTexture(const char* full_path)
{
	ILuint imageID;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(""))
	{
		ComponentMaterial* tex = new ComponentMaterial;

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
			glGenTextures(1, (GLuint*)tex->id_texture);
			glBindTexture(GL_TEXTURE_2D, (GLuint)tex->id_texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//Texture Specifications
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			
			comp_mats.push_back(tex);
			App->editor->properties->SaveTextureInfo(full_path, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), (void*)tex->id_texture);
		}
	}
	else {
		error = ilGetError();
	}

	
	ilDeleteImages(1, &imageID);
}

void ModuleMaterial::Draw()
{
	for (int i = 0; i < comp_mats.size(); i++)
	{
		glEnable(GL_TEXTURE_2D);

		/*glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].id_indices);*/

		/*glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].id_uvs);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);*/

		glBindTexture(GL_TEXTURE_2D, (GLuint)comp_mats[i]->id_texture);
		//glDrawElements(GL_TRIANGLES, meshes[i].num_indices, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisable(GL_TEXTURE_2D);
	}
}



void ModuleMaterial::DeleteTextures()
{
	if (comp_mats.size() > 0) {
		while (comp_mats.size() > 0)
		{
			glDeleteTextures(1, &comp_mats[comp_mats.size() - 1]->id_texture);
			comp_mats[comp_mats.size() - 1]->id_texture = NULL;

			App->editor->properties->EraseTextureInfo();

			comp_mats.pop_back();
		}

		App->editor->properties->EraseTextureInfo();

		CONSOLELOG("Old textures deleted");
	}
}

ComponentMaterial* ModuleMaterial::CreateComponentMaterial()
{
	ComponentMaterial* new_comp = new ComponentMaterial;
	comp_mats.push_back(new_comp);

	return new_comp;
}