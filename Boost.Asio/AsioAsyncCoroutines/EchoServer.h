#pragma once

#include <memory>
#include "EchoConnection.h"
#include "Profiler.h"

class EchoServer : public std::enable_shared_from_this<EchoServer>
{
public:

	static std::shared_ptr<EchoServer> make_shared(io_service&, int port);
	EchoServer(io_service&, int port);

	void run();

private:

	void run_may_throw();
	void start_new_connection(std::shared_ptr<EchoConnection>);

	Profiler _profiler;
	boost::asio::io_service& _io_service;
	tcp::acceptor _acceptor;
};

