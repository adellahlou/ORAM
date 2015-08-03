#pragma once

#include <chrono>

class Timer {
	using clock = std::chrono::steady_clock;
	
	clock::time_point startTime, stopTime;
	
	bool isRunning;
	
public:
	Timer();
	~Timer();
	
	void Start();
	void Stop();	
	void Reset();

	bool IsRunning() const;
	
	double GetElapsedTime();
};
