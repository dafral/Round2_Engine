#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	Uint32 Read();
	float ReadSec() const;
	void Reset();

	void PauseOn();
	void PauseOff();

	bool	running;

private:

	Uint32	started_at;
	Uint32	last_read;
	Uint32	stopped_at;
	bool	paused = false;
	Uint32  paused_at = 0;
};

#endif //__TIMER_H__