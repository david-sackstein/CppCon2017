#pragma once

#include "Messenger.h"
#include "Echoer.h"

class EchoConnection
{
public:

	EchoConnection(tcp::socket&&, std::function<void()> onEcho);

	void stop();

private:

	void run();

	tcp::socket _socket;
	Messenger _messenger;
	Echoer _echoer;
	std::thread _thread;
};

typedef std::shared_ptr<EchoConnection> EchoConnectionPtr;

