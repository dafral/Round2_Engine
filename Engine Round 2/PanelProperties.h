#ifndef __P_PROPERTIES_H__
#define __P_PROPERTIES_H__

#include "Panel.h"

class PanelProperties : public Panel
{
public:
	PanelProperties(bool active);
	virtual ~PanelProperties();

	void Draw();

	void SaveMeshInfo(const char* file_name, int nMesh, int nVertices);

	const char* gName = " ";
	int gNumMesh = 0;
	int gNumVert = 0;

};

#endif // !__P_PROPERTIES_H__