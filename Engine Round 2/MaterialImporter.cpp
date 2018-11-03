#include "MaterialImporter.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Devil/include/IL/il.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


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