// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Reset()
{
	running = false;
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	if (running == true)
	{
		return (float)(SDL_GetTicks() - started_at) / 1000;
	}
	else
	{
		return (float)(stopped_at - started_at) / 1000;
	}
}

void Timer::PauseOn()
{
	if (paused == false)
	{
		paused_at = SDL_GetTicks();
		paused = true;
	}
}

void Timer::PauseOff()
{
	if (paused == true)
	{
		started_at += (SDL_GetTicks() - paused_at);
		last_read += (SDL_GetTicks() - paused_at);

		paused_at = 0;
		paused = false;
	}
}


