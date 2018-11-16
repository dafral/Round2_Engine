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
			/*App->material_importer->SaveAsDDS();*/
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

void MaterialImporter::SaveAsDDS()
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
			char file[69];
			sprintf_s(file, "Library\\Materials\\texture_%d.dds", save_id++);
			FILE* tex_file = fopen(file, "wb");
			fwrite(data, sizeof(ILubyte), size, tex_file);
			fclose(tex_file);
			CONSOLELOG("New material texture saved: %s.", file);
		}
	}

}