#include "stdafx.h"
#include "EchoServer.h"
#include "Logger.h"

static Logger& logger = Logger::instance();

std::shared_ptr<EchoServer> EchoServer::make_shared(io_service& io_service, int port)
{
	return std::make_shared<EchoServer>(io_service, port);
}

EchoServer::EchoServer(io_service& io_service, int port) :
	_io_service(io_service),
	_acceptor(io_service, tcp::endpoint(tcp::v4(), port))
{
}

void EchoServer::run()
{
	try
	{
		run_may_throw();
	}
	catch (std::exception& ex)
	{
		logger.Log("server failed : %s\n", ex.what());
	}
}

void EchoServer::run_may_throw()
{
	auto connection = std::make_shared<EchoConnection>(
		_io_service,
		[this]() { _profiler.OnEcho(); });

	auto self(shared_from_this());

	spawn(_io_service, [self, connection, this](yield_context yield)
	{
		error_code ec;
		_acceptor.async_accept(connection->get_socket(), yield[ec]);
		if (!ec)
		{
			self->start_new_connection(connection);
		}
	});
}

void EchoServer::start_new_connection(std::shared_ptr<EchoConnection> connection)
{
	connection->start();
	run_may_throw();
}