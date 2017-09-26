#pragma once

#include <boost/asio.hpp>

#include "Message.h"

class AsyncMessenger
{
public:

	AsyncMessenger(tcp::socket& socket);

	Message read();
	void write(Message& message);

private:

	error_code read(char* data, size_t size);
	error_code write(char* data, size_t size);

	tcp::socket& _socket;
};
