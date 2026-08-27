/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:40:09 by manwar            #+#    #+#             */
/*   Updated: 2026/08/27 20:51:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "HttpConfig.hpp"
#include "IpAddressPortPairHelpers.hpp"
#include "Server.hpp"

// Enough for any decimal port number, including the terminator.
static const std::size_t PORT_STRING_SIZE = 16;
static const int LISTEN_BACKLOG = 1024;

Server::Server(const MainConfig &mainConfig): mainConfig(mainConfig)
{
}

Server::~Server(void)
{
	for (std::map<int, Client>::iterator iterator = clients.begin();
	     iterator != clients.end(); ++iterator)
	{
		close(iterator->first);
	}
	for (std::size_t i = 0; i < listenerFileDescriptors.size(); ++i)
	{
		if (listenerFileDescriptors[i] >= 0)
		{
			close(listenerFileDescriptors[i]);
		}
	}
}

bool Server::startServer(void)
{
	if (!mainConfig.httpConfigSettingResolves())
	{
		std::cerr << "No http block in config: nothing to serve." << std::endl;
		return false;
	}

	// Already deduplicated by address and port, and canonicalised, by the
	// config module.
	const std::vector<std::string> addressPortPairs =
		mainConfig.resolveHttpConfigSetting()
			.extractAllServerConfigSettingAddressPortPairs();

	for (std::size_t i = 0; i < addressPortPairs.size(); ++i)
	{
		const std::string &addressPortPair = addressPortPairs[i];
		const std::pair<std::string, uint16_t> addressAndPort =
			IpAddressPortPairHelpers::splitCanonicalIpAddressPortPair(
				addressPortPair);

		int fileDescriptor =
			createListener(addressAndPort.first, addressAndPort.second);
		if (fileDescriptor < 0)
		{
			return false;
		}

		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
		pollfd listenerPollFd;
		listenerPollFd.fd = fileDescriptor;
		listenerPollFd.events = POLLIN;
		listenerPollFd.revents = 0;

		listenerFileDescriptors.push_back(fileDescriptor);
		listenerAddressPortPairs[fileDescriptor] = addressPortPair;
		pollFds.push_back(listenerPollFd);

		std::cout << "Listening on " << addressPortPair
				  << std::endl; // TODO: Remove debug message
	}
	return !listenerFileDescriptors.empty();
}

void Server::runServer(void)
{
	for (;;)
	{
		// std::vector::data() is C++11, so the address of the first element
		// is the C++98 way to reach the underlying array.
		// NOLINTBEGIN(readability-container-data-pointer)
		int readyCount =
			poll(&pollFds[0], static_cast<nfds_t>(pollFds.size()), -1);
		// NOLINTEND(readability-container-data-pointer)
		if (readyCount < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			perror("poll");
			break;
		}
		for (std::size_t i = 0; i < pollFds.size() && readyCount > 0;)
		{
			if (pollFds[i].revents == 0)
			{
				++i;
				continue;
			}
			--readyCount;
			int fileDescriptor = pollFds[i].fd;
			if (listenerAddressPortPairs.find(fileDescriptor) !=
			    listenerAddressPortPairs.end())
			{
				acceptClient(fileDescriptor);
				++i;
			}
			else if (clientEventHandler(i))
			{
				++i;
			}
			// else: removeClient shifted the next clientPollFd into position i
		}
	}
}

bool Server::clientEventHandler(std::size_t pollFdIndex)
{
	short returnedEvents = pollFds[pollFdIndex].revents;
	int fileDescriptor = pollFds[pollFdIndex].fd;
	Client &client = clients[fileDescriptor];

	if (!client.getWhetherConnectionShouldClose() && (returnedEvents & POLLIN))
	{
		client.onRecv(fileDescriptor);
	}
	if (returnedEvents & POLLOUT)
	{
		client.onSend(fileDescriptor);
	}
	// A client that wants to close may still have a response queued, so hold
	// the connection open until everything buffered has been flushed.
	if ((returnedEvents & (POLLERR | POLLHUP)) ||
	    (client.getWhetherConnectionShouldClose() &&
	     !client.getWhetherOutputIsPending()))
	{
		removeClient(pollFdIndex);
		return false;
	}
	syncEvents(pollFdIndex, client);
	return true;
}

void Server::acceptClient(int listenerFileDescriptor)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	sockaddr_storage clientAddress;
	socklen_t clientAddressLength;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	pollfd clientPollFd;
	int clientFileDescriptor;
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	clientAddressLength = sizeof(clientAddress);
	// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
	clientFileDescriptor = accept(listenerFileDescriptor,
	                              reinterpret_cast<sockaddr *>(&clientAddress),
	                              &clientAddressLength);
	// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
	if (clientFileDescriptor < 0)
	{
		return;
	}
	if (setNonblock(clientFileDescriptor) < 0)
	{
		perror("fcntl");
		close(clientFileDescriptor);
		return;
	}
	clients[clientFileDescriptor] = Client();
	clients[clientFileDescriptor].setUp(
		mainConfig.resolveHttpConfigSetting(),
		listenerAddressPortPairs[listenerFileDescriptor]);

	clientPollFd.fd = clientFileDescriptor;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
	pollFds.push_back(clientPollFd);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	if (getnameinfo(reinterpret_cast<sockaddr *>(&clientAddress),
	                clientAddressLength, host, sizeof(host), service,
	                sizeof(service), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
	{
		std::cout << "Connected fd=" << clientFileDescriptor << " from " << host
				  << ":" << service << std::endl;
	}
}

void Server::removeClient(std::size_t pollFdIndex)
{
	int fileDescriptor = pollFds[pollFdIndex].fd;

	std::cout << "Removing fd=" << fileDescriptor << std::endl;
	std::map<int, Client>::const_iterator client = clients.find(fileDescriptor);
	if (client != clients.end() && client->second.getBodyFileDescriptor() >= 0)
	{
		close(client->second.getBodyFileDescriptor());
	}
	close(fileDescriptor);
	clients.erase(fileDescriptor);
	pollFds.erase(pollFds.begin() + static_cast<std::ptrdiff_t>(pollFdIndex));
}

/*
 *	register POLLOUT when _send_buf is non-empty
 *
 *	-> data gets queued into _send_buf during onRecv
 *	-> syncEvents registers POLLOUT
 *	-> next poll waks on writable
 *	-> onWritable flushes the buffer
 *	-> next syncEvents clears POLLOUT again
 *
 *
 */
void Server::syncEvents(std::size_t pollFdIndex, const Client &client)
{
	pollFds[pollFdIndex].events = POLLIN;
	if (client.getWhetherOutputIsPending())
	{
		pollFds[pollFdIndex].events |= POLLOUT;
	}
}

int Server::setNonblock(int fileDescriptor)
{
	int flags;

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	flags = fcntl(fileDescriptor, F_GETFL, 0);
	if (flags < 0)
	{
		return -1;
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	return fcntl(fileDescriptor, F_SETFL, flags | O_NONBLOCK);
}

int Server::createListener(const std::string &host, int port)
{
	char portString[PORT_STRING_SIZE];
	int fileDescriptor;
	int opt = 1;
	int err;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	addrinfo hints;
	addrinfo *addressInfo;

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	std::snprintf(portString, sizeof(portString), "%d", port);

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	const char *hostArg;

	if (host.empty())
	{
		hostArg = NULL;
	}
	else
	{
		// Pairs from the config module are already resolved to numeric form,
		// so no name lookup should happen here.
		hints.ai_flags |= AI_NUMERICHOST;
		hostArg = host.c_str();
	}

	err = getaddrinfo(hostArg, portString, &hints, &addressInfo);
	if (err != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(err) << std::endl;
		return -1;
	}

	fileDescriptor = socket(addressInfo->ai_family, addressInfo->ai_socktype,
	                        addressInfo->ai_protocol);
	if (fileDescriptor < 0)
	{
		perror("socket");
		freeaddrinfo(addressInfo);
		return -1;
	}
	setsockopt(fileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (addressInfo->ai_family == AF_INET6)
	{
		// Otherwise a wildcard IPv6 listener also claims the IPv4 port, and a
		// separately configured IPv4 listener on that port fails to bind.
		setsockopt(fileDescriptor, IPPROTO_IPV6, IPV6_V6ONLY, &opt,
		           sizeof(opt));
	}
	if (bind(fileDescriptor, addressInfo->ai_addr, addressInfo->ai_addrlen) < 0)
	{
		perror("bind");
		close(fileDescriptor);
		freeaddrinfo(addressInfo);
		return -1;
	}
	freeaddrinfo(addressInfo);

	if (listen(fileDescriptor, LISTEN_BACKLOG) < 0)
	{
		perror("listen");
		close(fileDescriptor);
		return -1;
	}
	if (setNonblock(fileDescriptor) < 0)
	{
		perror("fcntl");
		close(fileDescriptor);
		return -1;
	}
	return fileDescriptor;
}
