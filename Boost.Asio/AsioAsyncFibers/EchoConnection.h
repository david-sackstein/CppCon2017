#pragma once

#include "AsyncMessenger.h"
#include "Echoer.h"

class EchoConnection
{
public:

	EchoConnection(tcp::socket&&, std::function<void()> onEcho);

	void stop();

private:

	void run();

	tcp::socket _socket;
	AsyncMessenger _messenger;
	Echoer<AsyncMessenger> _echoer;
	boost::fibers::fiber _fiber;
};

typedef std::shared_ptr<EchoConnection> EchoConnectionPtr;

