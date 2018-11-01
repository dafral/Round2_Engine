#ifndef __P_INSPECTOR_H__
#define __P_INSPECTOR_H__

#include "Globals.h"
#include "Panel.h"

class GameObject;

class PanelInspector : public Panel
{
public:
	PanelInspector(bool active);
	virtual ~PanelInspector();

	void Init();
	void Draw();
	GameObject* GetSelectedGO() { return go_selected; };

private:
	void CreateHierarchy(GameObject* curr_go);

private:
	GameObject* go_selected = nullptr;
};

#endif
