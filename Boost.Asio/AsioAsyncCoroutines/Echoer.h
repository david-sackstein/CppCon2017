#pragma once

#include <thread>
#include "Messenger.h"
#include "Logger.h"

template <typename _OwnerType>
class Echoer
{
public:

	Echoer(Messenger& messenger, std::function<void()> onEcho) :
		_messenger(messenger),
		_onEcho(onEcho)
	{
	}

	void write_and_read(Message&& message, yield_context yield)
	{
		_sentMessage = std::move(message);

		while (true)
		{
			_messenger.async_write(_sentMessage, yield);

			Message readMessage = _messenger.async_read(yield);

			compare(readMessage);

			_sentMessage = std::move(readMessage);
		}
	}

	void read_and_write(yield_context yield)
	{
		Message readMessage = _messenger.async_read(yield);
		write_and_read(std::move(readMessage), yield);
	}

private:

	void Echoer::compare(const Message& readMessage)
	{
		if (_sentMessage == readMessage)
		{
			_onEcho();
		}
		else
		{
			_logger.Log("comparison failed\n");
		}
	}

	std::string ThreadIdString()
	{
		return IdToString(std::this_thread::get_id());
	}

	std::string IdToString(const std::thread::id& id)
	{
		std::ostringstream oss;
		oss << std::this_thread::get_id();
		return oss.str();
	}

	Messenger& _messenger;
	Message _sentMessage;
	std::function<void()> _onEcho;
	Logger& _logger = Logger::instance();
};
