#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"
#include "Component_Mesh.h"

class MeshImporter : public Importer<class Component_Mesh>
{
public:
	Component_Mesh* Load(const char * myfile);
	//bool Import(const char* path, Component_Mesh* meshes);
};

#endif // !__MESHIMPORTER_H__

