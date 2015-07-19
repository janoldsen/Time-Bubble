#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H


class __declspec(dllexport) GameClock
{
public:

	static GameClock* getInstance();

	void initialize();

	void shutdown();

	void startNewFrame();

	float getElapsedTime();
	__int64 getCurrTime();

	float getTotalElapsedTime();


private:
	GameClock() {}
	GameClock(GameClock&);
	GameClock& operator=(GameClock&);

	static GameClock instance;

	double frq;

	__int64 startTime;
	__int64 currTime;
	__int64 lastTime;

};


#define gameClock GameClock::getInstance()

#endif