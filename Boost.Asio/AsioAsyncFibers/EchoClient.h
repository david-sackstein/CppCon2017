#pragma once

#include <iostream>
#include <thread>
#include "Messenger.h"
#include "Echoer.h"

class EchoClient
{
public:

	EchoClient(int serverPort, int id);
	~EchoClient();

private:

	void run();
	void stop();

	void connect();
	void run_may_throw();

	Message CreateMessage();

	int _serverPort;
	int _id;

	std::atomic<bool> _is_running;
	boost::asio::io_service _io_service;
	tcp::socket _socket;
	Messenger _messenger;
	Echoer<Messenger> _echoer;
	std::thread _thread;
};
