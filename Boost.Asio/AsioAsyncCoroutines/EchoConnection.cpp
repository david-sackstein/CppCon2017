#include "stdafx.h"
#include <boost/asio/spawn.hpp>
#include "EchoConnection.h"
#include "Logger.h"

static Logger& logger = Logger::instance();

EchoConnection::EchoConnection(io_service& io_service, std::function<void()> onEcho) :
	_strand(io_service),
	_socket(io_service),
	_messenger(_socket),
	_echoer(_messenger, onEcho)
{
}

void EchoConnection::start()
{
	auto self(shared_from_this());

	spawn(_strand, [self, this](yield_context yield)
	{
		self->start(yield);
	});
}

void EchoConnection::start(yield_context yield)
{
	try
	{
		_echoer.read_and_write(yield);
	}
	catch (std::exception& ex)
	{
		logger.Log("connection failed : %s\n", ex.what());
	}
}
