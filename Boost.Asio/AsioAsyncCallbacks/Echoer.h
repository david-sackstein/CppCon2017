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

	void write_and_read(Message&& message, std::shared_ptr<_OwnerType> owner)
	{
		_sentMessage = std::move(message);

		_messenger.async_write(_sentMessage, [this, owner](const error_code& ec)
		{
			if (!ec)
			{
				loop(owner);
			}
		});
	}

	void read_and_write(std::shared_ptr<_OwnerType> owner)
	{
		loop(owner);
	}

private:

	void loop(std::shared_ptr<_OwnerType> owner)
	{
		_messenger.async_read([this, owner](const error_code& ec, Message&& readMessage)
		{
			if (!ec)
			{
				compare(readMessage);

				write_and_read(std::move(readMessage), owner);
			}
		});
	}

	void Echoer::compare(const Message& readMessage)
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
