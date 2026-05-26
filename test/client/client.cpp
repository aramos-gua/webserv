#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/*
*	client.cpp creates an arbitrary client to connect to a server on port 8000.
*	Once executed it expects a user input to send as a message to the server, which will then send a response back
*	Used for basic connection testing. Response buffer is 1024 chars.
*/


int	main(void)
{
	// Creating socket
	int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1)
	{
		std::cerr << "socket() failed, errno [" << errno << "]" << std::endl;
		exit (EXIT_FAILURE);
	}

	// Specifying address
	sockaddr_in	serverAddress;
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8000);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Sending connection request
	if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "connect() failed, errno [" << errno << "]" << std::endl;
		exit (EXIT_FAILURE);
	}

	// Sending data to the server
	std::string	msg;

	std::cin >> msg;
	if (send(clientSocket, msg.c_str(), msg.size(), 0) < 0)
	{
		std::cerr << "send() failed, errno [" << errno << "]" << std::endl;
		exit (EXIT_FAILURE);
	}


	// Receiving response
	char	buffer_recv[1024] = {0};

	if (recv(clientSocket, buffer_recv, sizeof(buffer_recv), 0) < 0);
	{
		std::cerr << "recv() failed, errno [" << errno << "]" << std::endl;
		exit (EXIT_FAILURE);
	}
	std::cout << "Response from server: " << buffer_recv << std::endl;

	close(clientSocket);
	return (0);
}
