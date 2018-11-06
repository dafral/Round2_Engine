#ifndef __P_GAME_H__
#define __P_GAME_H__

#include "Panel.h"

class PanelGame : public Panel
{
public:
	PanelGame(bool active);
	virtual ~PanelGame();

	void Init();
	void Draw();
};

#endif // !__P_GAME_H__
