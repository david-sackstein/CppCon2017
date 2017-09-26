#include "stdafx.h"
#include "Messenger.h"

Messenger::Messenger(tcp::socket& socket) :
	_socket(socket)
{
}

Message Messenger::read()
{
	size_t size;

	boost::asio::read(
		_socket,
		boost::asio::buffer(&size, sizeof(size)),
		boost::asio::transfer_all());

	Message message (size);

	boost::asio::read(
		_socket,
		boost::asio::buffer(message.data(), message.size()),
		boost::asio::transfer_all());

	return message;
}

void Messenger::write(Message& message)
{
	write(message.data(), message.size());
}

void Messenger::write(const char* data, size_t size)
{
	boost::asio::write(
		_socket,
		boost::asio::buffer(&size, sizeof(size)),
		boost::asio::transfer_all());

	boost::asio::write(
		_socket,
		boost::asio::buffer(data, size),
		boost::asio::transfer_all());
}
