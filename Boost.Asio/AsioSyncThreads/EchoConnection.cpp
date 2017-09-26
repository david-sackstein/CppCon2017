#include "stdafx.h"
#include "EchoConnection.h"
#include "Logger.h"

static Logger& logger = Logger::instance();

EchoConnection::EchoConnection(tcp::socket&& socket, std::function<void()> onEcho) :
	_socket(std::move(socket)),
	_messenger(_socket),
	_echoer(_messenger, onEcho),
	_thread(std::thread([this] { run(); }))
{
}

void EchoConnection::run()
{
	try
	{
		_echoer.read_and_write();
	}
	catch (std::exception& ex)
	{
		logger.Log("connection failed : %s\n", ex.what());
	}
}

void EchoConnection::stop()
{
	_echoer.cancel();
	_thread.join();
}