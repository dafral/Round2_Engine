#ifndef __PANELRESOURCES_H__
#define __PANELRESOURCES_H__

#include "Panel.h"

class PanelResources : public Panel
{
public:
	PanelResources(bool active);
	virtual ~PanelResources();
	
	void Init();
	void Draw();
};

#endif // !__PANELRESOURCES_H__

