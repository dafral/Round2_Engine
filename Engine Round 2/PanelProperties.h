#ifndef __P_PROPERTIES_H__
#define __P_PROPERTIES_H__

#include "Globals.h"
#include "Panel.h"
#include <string>
#include "ImGui/imgui.h"
#include "glmath.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

class PanelProperties : public Panel
{
public:
	PanelProperties(bool active);
	virtual ~PanelProperties();

	void Init();
	void Draw();

	void SaveTransformationInfo(aiVector3D pos, aiQuaternion rot, aiVector3D scale);
	void SaveMeshInfo(const char* file_name, int nMesh, int nVertices, int nTriangles);
	void SaveTextureInfo(const char* file_name, int width, int height, ImTextureID snapshot);
	void EraseGeometryInfo();
	void EraseTextureInfo();

	//Transform info
	vec3 position = (0, 0, 0);
	vec3 rotation = (0, 0, 0);
	vec3 scale = (0, 0, 0);

	//Mesh info
	const char* gName = " ";
	int gNumMesh = 0;
	int gNumVert = 0;
	int gNumTriangles = 0;

	//Texture info
	const char* tName = " ";
	std::string tType;
	int tWidth = 0;
	int tHeight = 0;
	ImTextureID tSnap = nullptr;

};

#endif // !__P_PROPERTIES_H__