#ifndef __P_INSPECTOR_H__
#define __P_INSPECTOR_H__

#include "Globals.h"
#include "Panel.h"

class GameObject;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(bool active);
	virtual ~PanelHierarchy();

	void Init();
	void Draw();
	GameObject* GetSelectedGO() { return go_selected; };
	void SetSelectedGO(GameObject* go) { go_selected = go; };

private:
	void CreateHierarchy(GameObject* curr_go);

private:
	GameObject* go_selected = nullptr;
};

#endif
