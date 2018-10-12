#ifndef __P_PROPERTIES_H__
#define __P_PROPERTIES_H__

#include "Panel.h"
#include <string>

class PanelProperties : public Panel
{
public:
	PanelProperties(bool active);
	virtual ~PanelProperties();

	void Draw();

	void SaveMeshInfo(const char* file_name, int nMesh, int nVertices, int nTriangles);
	void SaveTextureInfo(const char* file_name, int width, int height);

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

};

#endif // !__P_PROPERTIES_H__