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
	void Update();

	float2 GetMousePosNormalized();
};

#endif // !__PANEL_H__