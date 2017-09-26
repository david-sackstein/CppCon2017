#include "stdafx.h"
#include "EchoServer.h"
#include "Logger.h"

static Logger& logger = Logger::instance();

EchoServer::EchoServer(int port) :
	_acceptor(_io_service, tcp::endpoint(tcp::v4(), port)),
	_thread(std::thread([&] { run(); }))
{
}

EchoServer::~EchoServer()
{
	stop();
}

void EchoServer::run()
{
	try
	{
		for (;;)
		{
			run_may_throw();
		}
	}
	catch (std::exception& ex)
	{
		logger.Log("server failed : %s\n", ex.what());
	}
}

void EchoServer::stop()
{
	_acceptor.cancel();

	cancel_connections();

	if (_thread.joinable())
	{
		_thread.join();
	}
}

void EchoServer::run_may_throw()
{
	tcp::socket socket(_io_service);
	_acceptor.accept(socket);

	start_new_connection(std::move(socket));
}

void EchoServer::start_new_connection(tcp::socket&& socket)
{
	auto connection = std::make_shared<EchoConnection>(
		std::move(socket),
		[this]() { _profiler.OnEcho(); });

	_connections.insert(connection);
}

void EchoServer::cancel_connections()
{
	for (auto connection : _connections)
	{
		connection->stop();
	}
	_connections.clear();
}
