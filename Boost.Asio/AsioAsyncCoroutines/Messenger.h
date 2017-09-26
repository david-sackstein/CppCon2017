#pragma once

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include "Message.h"

class Messenger
{
public:

	Messenger(tcp::socket&);

	Message async_read(yield_context yield);
	void async_write(const Message& message, yield_context yield);

private:

	tcp::socket& _socket;
};
