#pragma once

#include <boost/asio.hpp>

#include "Message.h"

class Messenger
{
public:

	Messenger(tcp::socket& socket);

	Message read();
	void write(Message& message);

private:

	void write(const char* data, size_t size);

	tcp::socket& _socket;
};
