#ifndef __PANEL_H__
#define __PANEL_H__

class Panel
{
public:
	Panel(bool active = true) : active(active) {};
	virtual ~Panel() {};

	void Init();
	void CleanUp();
	bool isActive();

	void virtual Draw() {};

	bool* getActive() { return &active; };
	void switchActive() { active != active; };

	bool active;
};


#endif // !__PANEL_H__


