#include "stdafx.h"
#include "Echoer.h"
#include "Logger.h"

Echoer::Echoer(Messenger& messenger) :
	_is_running(true),
	_messenger(messenger)
{
}

void Echoer::read_and_write()
{
	loop();
}

void Echoer::cancel()
{
	_is_running = false;
}

void Echoer::loop()
{
	while (_is_running)
	{
		Message readMessage = _messenger.read();

		compare(readMessage);

		write(std::move(readMessage));
	}
}

void Echoer::compare(const Message& readMessage)
{
	if (!_sentMessage.data())
		return;

	if (_sentMessage == readMessage)
	{
		_logger.Log("thread %s data %d\n", ThreadIdString().c_str(), (int)_sentMessage.data()[0]);
	}
	else
	{
		_logger.Log("comparison failed\n");
	}
}

std::string Echoer::ThreadIdString()
{
	return IdToString(std::this_thread::get_id());
}

std::string Echoer::IdToString(std::thread::id& id)
{
	std::ostringstream oss;
	oss << std::this_thread::get_id();
	return oss.str();
}
