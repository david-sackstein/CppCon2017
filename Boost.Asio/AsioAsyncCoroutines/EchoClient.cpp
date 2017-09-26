#include "stdafx.h"
#include <boost/asio/spawn.hpp>
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
	_strand(io_service),
	_socket(_io_service),
	_messenger(_socket),
	_echoer(_messenger, [](){})
{
}

void EchoClient::run()
{
	auto self(shared_from_this());

	boost::asio::spawn(_strand, [self](yield_context yield)
	{
		self->run(yield);
	});
}

void EchoClient::run(yield_context yield)
{
	try
	{
		tcp::resolver::query query("localhost", std::to_string(_serverPort));
		tcp::resolver resolver(_io_service);
		_iterator = resolver.resolve(query);

		async_connect(yield);
		write_and_read(yield);
	}
	catch (std::exception& ex)
	{
		logger.Log("client failed %s\n", ex.what());
	}
}

void EchoClient::write_and_read(yield_context yield)
{
	_echoer.write_and_read(CreateMessage(), yield);
}

void EchoClient::async_connect(yield_context yield)
{
	error_code ec;

	do 
	{
		boost::asio::async_connect(_socket, _iterator, yield[ec]);
	} while (ec);
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
