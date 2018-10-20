#ifndef __P_INSPECTOR_H__
#define __P_INSPECTOR_H__

#include "Panel.h"

class PanelInspector : public Panel
{
public:
	PanelInspector(bool active);
	virtual ~PanelInspector();

	void Init();
	void Draw();

};

#endif
