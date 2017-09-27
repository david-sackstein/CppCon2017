#include "stdafx.h"
#include "AsyncMessenger.h"

AsyncMessenger::AsyncMessenger(tcp::socket& socket) :
	_socket(socket)
{
}

Message AsyncMessenger::read()
{
	size_t size;
	read(reinterpret_cast<char*>(&size), sizeof(size));

	Message message(size);
	read(message.data(), size);

	return message;
}

void AsyncMessenger::write(Message& message)
{
	size_t size = message.size();
	write(reinterpret_cast<char*>(&size), sizeof(size));

	write(message.data(), size);
}

error_code AsyncMessenger::read(char* data, size_t size)
{
	boost::fibers::promise<error_code> pr;
	boost::fibers::future<error_code> fu = pr.get_future();

	boost::asio::async_read(
		_socket,
		buffer(data, size),
		transfer_all(),
		[&](const error_code& ec, size_t bytes_transferred)
		{
			pr.set_value(ec);
		});

	return fu.get();
}

error_code AsyncMessenger::write(char* data, size_t size)
{
	boost::fibers::promise<error_code> pr;
	boost::fibers::future<error_code> fu = pr.get_future();

	boost::asio::async_write(
		_socket,
		buffer(data, size),
		transfer_all(),
		[&](const error_code& ec, size_t bytes_transferred)
	{
		pr.set_value(ec);
	});

	return fu.get();
}