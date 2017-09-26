#include "stdafx.h"
#include "Messenger.h"

Messenger::Messenger(tcp::socket& socket) :
	_socket(socket)
{
}

void Messenger::async_read(std::function<void(const error_code&, Message&&)> handler)
{
	auto callback = [this, handler](const error_code& ec, size_t bytes_transferred)
	{
		if (ec)
			handler(ec, 0);
		else
		{
			_message = Message(_size);

			boost::asio::async_read(
				_socket,
				boost::asio::buffer(_message.data(), _message.size()),
				boost::asio::transfer_all(),
				[this, handler](const error_code ec, size_t bytes_transferred)
				{
					handler(ec, std::move(_message));
				});
		}
	};

	boost::asio::async_read(
		_socket,
		buffer(&_size, sizeof(_size)),
		transfer_all(),
		callback);
}

void Messenger::async_write(const Message& message, std::function<void(const error_code&)> handler)
{
	const Message* pMessage = &message;

	auto callback = [this, handler, pMessage](const error_code& ec, size_t bytes_transferred)
	{
		if (ec) 
			handler(ec);
		else
		{
			boost::asio::async_write(
				_socket,
				buffer(pMessage->data(), pMessage->size()),
				transfer_all(),
				[handler](const error_code& ec, size_t bytes_transferred)
				{
					handler(ec);
				});

		}
	};

	_size = pMessage->size();

	boost::asio::async_write(
		_socket,
		buffer(&_size, sizeof(_size)),
		transfer_all(),
		callback);
}
