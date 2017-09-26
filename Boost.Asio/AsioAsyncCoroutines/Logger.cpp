#include "stdafx.h"
#include "Logger.h"

Logger& Logger::instance()
{
	static Logger logger([](const char* text) { printf(text); });
	return logger;
}

Logger::Logger(std::function<void(const char*)> logFunc) :
	_logFunc (logFunc)
{
}

void Logger::Log(const char* format, ...)
{
	std::lock_guard<std::mutex> lock(_mutex);

	va_list args;
	va_start(args, format);

	Format(format, args);

	_logFunc(&_data[0]);

	va_end(args);
}

void Logger::Format(const char* format, va_list args)
{
	int lengthWithoutNull = vsnprintf(0, 0, format, args);

	int lengthWithNull = 1 + lengthWithoutNull;

	_data.resize(lengthWithNull);

	int written = vsnprintf(&_data[0], lengthWithNull, format, args);

	_data[lengthWithoutNull] = 0;
}
