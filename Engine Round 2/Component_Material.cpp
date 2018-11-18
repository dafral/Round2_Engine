#include "Application.h"
#include "Component_Material.h"
#include "GameObject.h"

#include "PCG/pcg_basic.h"

#include "Devil/include/IL/il.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

Component_Material::Component_Material() : Component(MATERIAL)
{
	unique_id = pcg32_random_r(&App->rng);
}

Component_Material::Component_Material(uint id, uint w, uint h) : Component(MATERIAL)
{
	material.texture_id = id;
	material.width = w;
	material.height = h;
	unique_id = pcg32_random_r(&App->rng);
}

Component_Material::Component_Material(uint uid, uint id, uint w, uint h) : Component(MATERIAL)
{
	unique_id = uid;
	material.texture_id = id;
	material.width = w;
	material.height = h;
	unique_id = pcg32_random_r(&App->rng);
}

void Component_Material::LoadBuffers()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, (GLuint*) &material.texture_id);
	glBindTexture(GL_TEXTURE_2D, (GLuint)material.texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	material.width = ilGetInteger(IL_IMAGE_WIDTH);
	material.height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	//Texture Specifications
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), material.width, material.height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	snap = (void*)material.texture_id;
}

// -----------------------------------------------------------------

void Component_Material::OnSave(JSON_Doc* filetosave)
{
	filetosave->AddSectionToArray("Components");
	filetosave->MoveToSectionFromArray("Components", filetosave->GetArraySize("Components") - 1);

	filetosave->SetNumber("type", 2);
	filetosave->SetNumber("owner", my_go->GetUniqueID());

	filetosave->SetNumber("material", GetUniqueID());

}
