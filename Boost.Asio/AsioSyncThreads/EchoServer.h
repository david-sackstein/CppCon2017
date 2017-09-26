#pragma once

#include <set>
#include "EchoConnection.h"
#include "Profiler.h"

class EchoServer
{
public:

	EchoServer(int port);
	~EchoServer();

private:

	void run();
	void run_may_throw();
	void start_new_connection(tcp::socket&&);

	void stop();
	void cancel_connections();

	Profiler _profiler;
	boost::asio::io_service _io_service;
	tcp::acceptor _acceptor;
	std::set<EchoConnectionPtr> _connections;
	std::thread _thread;
};

