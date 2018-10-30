#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"

class MeshImporter : public Importer<class Mesh>
{
public:
	Mesh* Load(const char * exported_file);
	bool Save(const char* path, Mesh* meshes);

	void ImportAllMeshes();
};

#endif // !__MESHIMPORTER_H__

