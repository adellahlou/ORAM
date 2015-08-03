#include "Timer.hpp"

using namespace std::chrono;

Timer::Timer()
{
	Reset();
}

Timer::~Timer()
{}

void Timer::Start()
{
	startTime = clock::now();
	isRunning = true;
}

void Timer::Stop()
{
	stopTime = clock::now();
	isRunning = false;
}

void Timer::Reset()
{
	// The current elapsed time is 0
	startTime = clock::now();
	stopTime = startTime;
	
	isRunning = false;
}

bool Timer::IsRunning() const
{
	return isRunning;
}

double Timer::GetElapsedTime()
{
	if (isRunning) {
		stopTime = clock::now();
	}
	
	// Time in seconds as a double
	auto elapsedTime = duration_cast<duration<double>>(stopTime - startTime);
	return elapsedTime.count();
}
