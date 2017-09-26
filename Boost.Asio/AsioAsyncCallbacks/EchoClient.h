#pragma once

#include "Messenger.h"
#include "Echoer.h"

class EchoClient : public std::enable_shared_from_this<EchoClient>
{
public:

	static std::shared_ptr<EchoClient> make_shared(io_service&, int serverPort, int id);

	void run();

private:

	EchoClient(io_service&, int serverPort, int id);

	void async_connect(std::function<void()>);
	void run_may_throw();
	void write_and_read();

	Message CreateMessage();

	int _serverPort;
	int _id;

	tcp::resolver::iterator _iterator;
	boost::asio::io_service& _io_service;
	tcp::socket _socket;
	Messenger _messenger;
	Echoer<EchoClient> _echoer;
	Logger& logger = Logger::instance();
};
