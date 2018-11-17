#ifndef __PANEL_H__
#define __PANEL_H__

#include "ImGui/imgui.h"
#include "ImGui/imgui_dock.h"

class Panel
{
public:
	Panel(bool active = true) : active(active) {};
	virtual ~Panel() {};

	void Init();
	void CleanUp();
	void virtual Draw() {};

	void switchActive() { active != active; };
	void SetHovered(bool new_hovered) { hovered = new_hovered; };
	bool* isActive() { return &active; };
	bool isHovered() { return hovered; };

private:
	bool hovered = false;
	bool active;
};


#endif // !__PANEL_H__


