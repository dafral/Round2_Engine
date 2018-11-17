#ifndef __P_SCENE_H__
#define __P_SCENE_H__

#include "Panel.h"
#include "MathGeoLib/MathGeoLib.h"

class PanelScene : public Panel
{
public:
	PanelScene(bool active);
	virtual ~PanelScene();

	void Init();
	void Draw();

	float2 GetMousePosNormalized();

private:
	ImVec2 window_pos = { 0, 0 };
	ImVec2 region = { 0, 0 };
};

#endif // !__PANEL_H__