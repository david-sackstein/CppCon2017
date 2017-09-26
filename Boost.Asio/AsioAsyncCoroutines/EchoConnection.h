#pragma once

#include <boost/asio/spawn.hpp>
#include "Messenger.h"
#include "Echoer.h"

class EchoConnection : public std::enable_shared_from_this<EchoConnection>
{
public:

	EchoConnection(io_service&, std::function<void()> onEcho);
	tcp::socket& get_socket() { return _socket; }

	void start();

private:

	void start(yield_context);

	io_service::strand _strand;
	tcp::socket _socket;
	Messenger _messenger;
	Echoer<EchoConnection> _echoer;
};
