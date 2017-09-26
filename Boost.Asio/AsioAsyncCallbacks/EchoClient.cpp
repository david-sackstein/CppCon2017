#include "stdafx.h"
#include "EchoClient.h"
#include "Logger.h"

std::shared_ptr<EchoClient> EchoClient::make_shared(io_service& io_service, int serverPort, int id)
{
	return std::shared_ptr<EchoClient>(new EchoClient(io_service, serverPort, id));
}

EchoClient::EchoClient(io_service& io_service, int serverPort, int id) :
	_serverPort(serverPort),
	_id(id),
	_io_service(io_service),
	_socket(_io_service),
	_messenger(_socket),
	_echoer(_messenger, [](){})
{
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
	tcp::resolver::query query("localhost", std::to_string(_serverPort));
	tcp::resolver resolver(_io_service);
	_iterator = resolver.resolve(query);

	auto self(shared_from_this());

	async_connect([self]()
	{
		self->write_and_read();
	});
}

void EchoClient::write_and_read()
{
	_echoer.write_and_read(CreateMessage(), shared_from_this());
}

void EchoClient::async_connect(std::function<void()> handler)
{
	boost::asio::async_connect(_socket, _iterator,
		[this, handler](const error_code ec, tcp::resolver::iterator i)
	{
		if (!ec)
		{
			handler();
		}
		else
		{
			async_connect(handler);
		}
	});
}

Message EchoClient::CreateMessage()
{
	Message message(256);

	char* data = message.data();
	for (size_t i = 0; i < message.size(); i++)
	{
		data[i] = _id;
	}
	return message;
}
