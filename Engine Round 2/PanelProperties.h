#ifndef __P_PROPERTIES_H__
#define __P_PROPERTIES_H__

#include "Panel.h"

class PanelProperties : public Panel
{
public:
	PanelProperties(bool active);
	virtual ~PanelProperties();

	void Draw();
};

#endif // !__P_PROPERTIES_H__