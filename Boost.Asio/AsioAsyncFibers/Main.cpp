#include "stdafx.h"
#include "EchoClient.h"
#include "EchoServer.h"

char WaitForChar();

void main()
{
	int serverPort = 64002;
	
	EchoClient client1(serverPort, 1);

	EchoClient client2(serverPort, 2);

	EchoClient client3(serverPort, 3);

	EchoServer server(serverPort);

	WaitForChar();
}

char WaitForChar()
{
	boost::fibers::unbuffered_channel<char> channel;

	std::thread canceller([&] {
		char c = getchar();
		channel.push(c);
	});

	canceller.detach();

	char c;
	channel.pop(c);
	return c;
}
