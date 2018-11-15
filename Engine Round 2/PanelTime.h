#ifndef  __PANELTIME_H__
#define  __PANELTIME_H__

#include "Panel.h"

class PanelTime : public Panel
{
public:
	PanelTime(bool active);
	virtual ~PanelTime();

	void Init();
	void Draw();
};


#endif // ! __PANELTIME_H__
