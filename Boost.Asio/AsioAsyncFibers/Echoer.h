#pragma once

#include <thread>
#include <atomic>
#include "Logger.h"

template <typename Messenger>
class Echoer
{
public:

	Echoer(Messenger& messenger, std::function<void()> onEcho) :
		_is_running(true),
		_messenger(messenger),
		_onEcho(onEcho)
	{
	}

	void write_and_read(Message&& message)
	{
		write(std::move(message));
		loop();
	}

	void read_and_write()
	{
		loop();
	}

	void cancel()
	{
		_is_running = false;
	}

private:

	void loop()
	{
		while (_is_running)
		{
			Message readMessage = _messenger.read();

			compare(readMessage);

			write(std::move(readMessage));
		}
	}

	void compare(const Message& readMessage)
	{
		if (!_sentMessage.data())
			return;

		if (_sentMessage == readMessage)
		{
			_onEcho();
		}
		else
		{
			_logger.Log("comparison failed\n");
		}
	}

	void write(Message&& message)
	{
		_sentMessage = std::move(message);
		_messenger.write(_sentMessage);
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
	
	std::atomic<bool> _is_running;
	Messenger& _messenger;
	Message _sentMessage;
	std::function<void()> _onEcho;
	Logger& _logger = Logger::instance();
};
