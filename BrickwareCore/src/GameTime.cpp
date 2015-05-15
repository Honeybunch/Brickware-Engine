#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/GameTime.hpp"

using namespace Brickware;
using namespace Core;

using namespace std::chrono;

long long GameTime::startTime = duration_cast<milliseconds>
								(high_resolution_clock::now().time_since_epoch()).count();

long long GameTime::frameStartTime;
long long GameTime::fixedFrameStartTime;

float GameTime::deltaTime = 1.0f/60.0f;
float GameTime::timeScale = 1.0f;

float GameTime::GetTimeScale()
{
	return timeScale;
}

void GameTime::SetTimeScale(float timeScale)
{
	GameTime::timeScale = timeScale;
}

float GameTime::GetDeltaTime()
{
	return deltaTime;
}
float GameTime::GetFixedDeltaTime()
{
	return 1.0f/60.0f;
}

long long GameTime::GetMillisSinceStart()
{
	return (duration_cast<milliseconds>
		(high_resolution_clock::now().time_since_epoch()).count() - startTime);
}

void GameTime::frameStart()
{
	frameStartTime = duration_cast<milliseconds>
		(high_resolution_clock::now().time_since_epoch()).count();
}

void GameTime::frameEnd()
{
	//If the frame start time is 0 we just have to drop this
	//Or else our delta time will be huge
	if (frameStartTime == 0)
		return;

	long long frameDifference = duration_cast<milliseconds>
		(high_resolution_clock::now().time_since_epoch()).count() - frameStartTime;

	deltaTime = frameDifference/1000.0f;
}