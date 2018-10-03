#ifndef __P_SCENE_H__
#define __P_SCENE_H__

#include "Panel.h"

class PanelScene : public Panel
{
public:
	PanelScene(bool active);
	virtual ~PanelScene();

	void Draw();
};

#endif // !__PANEL_H__