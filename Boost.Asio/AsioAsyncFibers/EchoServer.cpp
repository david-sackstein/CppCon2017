#include "stdafx.h"
#include "EchoServer.h"
#include "Logger.h"

static Logger& logger = Logger::instance();

EchoServer::EchoServer(int port) :
	_acceptor(_io_service, tcp::endpoint(tcp::v4(), port)),
	_isStopped(false),
	_fiber(boost::fibers::fiber([this] { run(); })),
	_schedulerFiber([this]() {
		while (!_isStopped)
		{
			_io_service.run_one();
			boost::this_fiber::yield();
		}
	})
{
	boost::this_fiber::yield();
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

	_isStopped = true;

	_fiber.join();
	_schedulerFiber.join();
}

void EchoServer::run_may_throw()
{
	tcp::socket socket(_io_service);

	boost::fibers::promise<error_code> pr;
	boost::fibers::future<error_code> fu = pr.get_future();

	_acceptor.async_accept(socket, [&](const error_code& ec)
	{
		pr.set_value(ec);
	});

	fu.get();

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
