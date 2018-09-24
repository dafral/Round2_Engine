#ifndef __P_ABOUT_H__
#define __P_ABOUT_H__

#include "Panel.h"

class PanelAbout : public Panel
{
public:
	PanelAbout(bool active);
	virtual ~PanelAbout();

	void Draw();
};

#endif