#pragma once

#include <stdarg.h>
#include <vector>
#include <functional>
#include <mutex>

class Logger
{
public:

	static Logger& instance();

	void Log(const char* format, ...);

private:

	Logger(std::function<void(const char*)> logFunc);

	void Format(const char* format, va_list args);

	std::vector<char> _data;
	std::function<void(const char*)> _logFunc;
	std::mutex _mutex;
};