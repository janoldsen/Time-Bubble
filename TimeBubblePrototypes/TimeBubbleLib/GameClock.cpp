#include "GameClock.h"
#include <windows.h>



GameClock GameClock::instance;

void GameClock::initialize()
{
	LARGE_INTEGER frequency;


	QueryPerformanceFrequency(&frequency);


	frq = double(frequency.QuadPart);

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	currTime = time.QuadPart;

	startTime = currTime;

}


void GameClock::startNewFrame()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	lastTime = currTime;

	currTime = time.QuadPart;
}


float GameClock::getElapsedTime()
{
	//LARGE_INTEGER time;
	//QueryPerformanceCounter(&time);

	return double(currTime - lastTime) / frq;

}

float GameClock::getTotalElapsedTime()
{

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);



	return double(time.QuadPart - startTime) / frq;

}

GameClock* GameClock::getInstance()
{
	return &instance;
}


__int64 GameClock::getCurrTime()
{
	return currTime;
}