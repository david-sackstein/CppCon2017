#include "stdafx.h"
#include <conio.h>
#include "EchoClient.h"
#include "EchoServer.h"

void main()
{
	int serverPort = 64002;
	io_service io_service;

	std::shared_ptr<EchoClient> pClient1 = EchoClient::make_shared(io_service, serverPort, 1);
	pClient1->run();

	std::shared_ptr<EchoClient> pClient2 = EchoClient::make_shared(io_service, serverPort, 2);
	pClient2->run();

	std::shared_ptr<EchoClient> pClient3 = EchoClient::make_shared(io_service, serverPort, 3);
	pClient3->run();

	std::shared_ptr<EchoServer> pServer = EchoServer::make_shared(io_service, serverPort);
	pServer->run();

	std::vector<std::thread> threads;

	for (int i = 0; i<5; i++)
	{
		threads.push_back(std::thread([&io_service] { io_service.run(); }));
	}

	std::getchar();

	io_service.stop();

	for (auto& t : threads)
	{
		t.join();
	}
}