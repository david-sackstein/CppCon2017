#include "stdafx.h"
#include "EchoConnection.h"
#include "Logger.h"

static Logger& logger = Logger::instance();

EchoConnection::EchoConnection(io_service& io_service, std::function<void()> onEcho) :
	_socket(io_service),
	_messenger(_socket),
	_echoer(_messenger, onEcho)
{
}

void EchoConnection::start()
{
	try
	{
		auto self(shared_from_this());

		_echoer.read_and_write(self);
	}
	catch (std::exception& ex)
	{
		logger.Log("connection failed : %s\n", ex.what());
	}
}
