#include "stdafx.h"
#include "Messenger.h"

Messenger::Messenger(tcp::socket& socket) :
	_socket(socket)
{
}

Message Messenger::async_read(yield_context yield)
{
	size_t size;

	size_t bytes_transferred = 
		boost::asio::async_read(
			_socket,
			buffer(&size, sizeof(size)),
			transfer_all(), yield);

	auto message = Message(size);

	bytes_transferred = boost::asio::async_read(
		_socket,
		boost::asio::buffer(message.data(), message.size()),
		boost::asio::transfer_all(),
		yield);

	return std::move(message);
}

void Messenger::async_write(const Message& message, yield_context yield)
{
	size_t size = message.size();

	boost::asio::async_write(
		_socket,
		buffer(&size, sizeof(size)),
		transfer_all(), yield);

	boost::asio::async_write(
		_socket,
		buffer(message.data(), message.size()),
		transfer_all(),
		yield);
}
