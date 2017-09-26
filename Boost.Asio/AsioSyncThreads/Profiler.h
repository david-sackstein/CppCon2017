#pragma once

#include <chrono>

class Profiler
{
public:

	Profiler();

	void OnEcho();

private:
	
	std::chrono::steady_clock::time_point _start;
	std::atomic<int> _count;
};
