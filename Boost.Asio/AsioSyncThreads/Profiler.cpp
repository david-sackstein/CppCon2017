#include "stdafx.h"
#include "Profiler.h"
#include "Logger.h"

using namespace std::chrono;

static Logger& _logger = Logger::instance();

Profiler::Profiler() :
	_count(0)
{
}

void Profiler::OnEcho()
{
	int count = _count++;

	if (count == 0)
	{
		_start = steady_clock::now();
		return;
	}

	if (count % 50000 == 0)
	{
		auto duration = duration_cast<milliseconds>(steady_clock::now() - _start);
		double rate = 1000 * _count / static_cast<double>(duration.count() + 1); // + 1 to prevent division by zero

		_logger.Log("%f echoes per second\n", rate);
	}
}
