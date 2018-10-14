#ifndef __P_CONSOLE_H__
#define __P_CONSOLE_H__

#include "Panel.h"

class PanelConsole : public Panel
{
public:
	PanelConsole(bool active);
	virtual ~PanelConsole();

	void Init();
	void Draw();

	void ConsoleLog(const char* log);

};

#endif
