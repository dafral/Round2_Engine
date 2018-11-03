#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Importer.h"
#include "Component_Material.h"

class MaterialImporter : public Importer<class Component_Material>
{
public:
	//Component_Material* Load(const char * myfile);
	void SaveAsDDS();

	int save_id = 0;
};

#endif // !__MATERIALIMPORTER_H__

