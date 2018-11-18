#include "Globals.h"
#include "Application.h"
#include "MaterialImporter.h"
#include "ModuleFileSystem.h"
#include "PanelProperties.h"

#include "Devil/include/IL/il.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


//Constructor
MaterialImporter::MaterialImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
MaterialImporter::~MaterialImporter()
{}

bool MaterialImporter::Start()
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

bool MaterialImporter::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void MaterialImporter::Import(const char* full_path, GameObject* go)
{
	ILuint imageID;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(full_path))
	{
		Component_Material* tex = CreateComponentMaterial(go);

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
			tex->LoadBuffers();
			std::string extension = App->filesystem->GetExtension(full_path);

			if (extension == "DDS" || extension == "dds")
			{
				App->filesystem->CopyFileTo(full_path, App->filesystem->library_material_path.c_str());

				std::string new_name = App->filesystem->library_material_path;
				new_name += std::to_string(tex->GetUniqueID());
				new_name += ".dds";

				std::string old_name = App->filesystem->library_material_path;
				old_name += App->filesystem->GetNameWithoutPath(full_path, false);

				std::rename(old_name.c_str(), new_name.c_str());
			}
			else 
			{
				App->material_importer->SaveAsDDS(tex->GetUniqueID());
			}

		}
	}
	else {
		error = ilGetError();
	}


	ilDeleteImages(1, &imageID);
}

// =====================================================================
// Component material
// =====================================================================

Component_Material* MaterialImporter::CreateComponentMaterial(GameObject* my_go)
{
	Component_Material* cmaterial = new Component_Material();
	cmaterial->my_go = my_go;
	my_go->AddComponent(cmaterial);
	materials.push_back(cmaterial);
	return cmaterial;
}

void MaterialImporter::DeleteTextures()
{
	//if (comp_mats.size() > 0) {
	//	while (comp_mats.size() > 0)
	//	{
	//		glDeleteTextures(1, &comp_mats[comp_mats.size() - 1]->id_texture);
	//		comp_mats[comp_mats.size() - 1]->id_texture = NULL;

	//		App->editor->properties->EraseTextureInfo();

	//		comp_mats.pop_back();
	//	}

	//	App->editor->properties->EraseTextureInfo();

	//	CONSOLELOG("Old textures deleted");
	//}
}

void MaterialImporter::SaveAsDDS(uint uid)
{
	ILuint		size;
	ILubyte*	data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);

	size = ilSaveL(IL_DDS, NULL, 0);
	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			std::string file = App->filesystem->library_material_path;
			file += std::to_string(uid);
			file += ".dds";

			FILE* tex_file = fopen(file.c_str(), "wb");
			fwrite(data, sizeof(ILubyte), size, tex_file);
			fclose(tex_file);

			CONSOLELOG("New material texture saved: %s.", file);
		}
	}
}

uint MaterialImporter::LoadTextureInMemory(uint w, uint h, GLubyte * tex_data, GLint format) const
{
	uint buff_id = 0;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLELOG("Error loading texture in memory: %s\n", gluErrorString(error));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &buff_id);
	glBindTexture(GL_TEXTURE_2D, buff_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, tex_data);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLELOG("Error loading texture in memory: %s\n", gluErrorString(error));
	}

	return buff_id;
}

void MaterialImporter::CheckSaveID(const char * file)
{
	std::string f = file;
	uint cut = f.find_last_of("_");
	std::string num = f.substr(cut + 1);

	int id = atoi(num.c_str());
	if (id > save_id)
		save_id = id + 1;
}