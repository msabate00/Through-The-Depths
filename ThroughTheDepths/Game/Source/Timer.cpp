// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL_timer.h"
	
Timer::Timer()
{
	startTime = -1000000;
	//Start();
}

void Timer::Start()
{
	startTime = SDL_GetTicks();
}

uint32 Timer::ReadSec() const
{
	return (SDL_GetTicks() - startTime) / 1000;
}

float Timer::ReadMSec() const
{
	return (float)(SDL_GetTicks() - startTime);
}

