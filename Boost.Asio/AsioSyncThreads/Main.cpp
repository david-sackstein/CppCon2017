#include "stdafx.h"
#include "EchoClient.h"
#include "EchoServer.h"

void main()
{
	int serverPort = 64002;

	EchoClient client1(serverPort, 1);

	EchoClient client2(serverPort, 2);

	EchoClient client3(serverPort, 3);

	EchoServer server(serverPort);

	std::getchar();
}