#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Module.h"
#include "Component_Mesh.h"
#include "MeshImporter.h"
#include "GameObject.h"

class GameObject;
class Component_Mesh;
struct aiMesh;
struct aiScene;
struct aiNode;

class MeshImporter : public Module
{
public:

	MeshImporter(Application* app, bool start_enabled = true);
	~MeshImporter();

	void ImportScene(const char* full_path, const char* file_name);
	void ImportMesh(const char* full_path, GameObject* parent, const aiScene* scene, aiNode* node);

	Component_Mesh* Load(const char * myfile);
	bool Save(const char* path, Component_Mesh* meshes);


};

#endif // !__MESHIMPORTER_H__

