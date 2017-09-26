#pragma once

#include <functional>
#include <boost/asio.hpp>

#include "Message.h"

class Messenger
{
public:

	Messenger(tcp::socket&);

	void async_read(std::function<void(const error_code&, Message&&)> handler);
	void async_write(const Message& message, std::function<void(const error_code&)> handler);

private:

	tcp::socket& _socket;
	size_t _size;
	Message _message;
};
