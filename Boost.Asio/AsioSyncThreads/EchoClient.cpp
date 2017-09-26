#include "stdafx.h"
#include "EchoClient.h"
#include "Logger.h"

static Logger& logger = Logger::instance();

EchoClient::EchoClient(int serverPort, int id) :
	_is_running(true),
	_serverPort(serverPort),
	_id(id),
	_socket(_io_service),
	_messenger (_socket),
	_echoer (_messenger, [](){}),
	_thread (std::thread([&] { run(); }))
{
}

EchoClient::~EchoClient()
{
	stop();
}

void EchoClient::run()
{
	try
	{
		run_may_throw();
	}
	catch (std::exception& ex)
	{
		logger.Log("client failed %s\n", ex.what());
	}
}

void EchoClient::run_may_throw()
{
	connect();

	_echoer.write_and_read(CreateMessage());
}

void EchoClient::stop()
{
	_is_running = false;
	_echoer.cancel();

	if (_thread.joinable())
	{
		_thread.join();
	}
}

void EchoClient::connect()
{
	tcp::resolver::query query("localhost", std::to_string(_serverPort));
	tcp::resolver resolver(_io_service);
	auto iterator = resolver.resolve(query);

	_is_running = true;

	while (_is_running)
	{
		boost::system::error_code ec;
		boost::asio::connect(_socket, iterator, ec);
		if (!ec)
			break;
	}
}

Message EchoClient::CreateMessage()
{
	Message message (256);

	char* data = message.data();
	for (size_t i = 0; i < message.size(); i++)
	{
		data[i] = _id;
	}
	return message;
}
