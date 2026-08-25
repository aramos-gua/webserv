/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manwar <manwar@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:40:09 by manwar            #+#    #+#             */
/*   Updated: 2026/08/25 21:40:10 by manwar           ###   ########.fr       */
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

Server::Server(const MainConfig &mainConfig): _mainConfig(mainConfig)
{
}

Server::~Server()
{
	for (std::map<int, Client>::iterator iterator = _clients.begin();
	     iterator != _clients.end(); ++iterator)
	{
		close(iterator->first);
	}
	for (size_t i = 0; i < _listeners.size(); ++i)
	{
		if (_listeners[i] >= 0)
		{
			close(_listeners[i]);
		}
	}
}

bool Server::startServer()
{
	if (!_mainConfig.getWhetherHttpConfigSet())
	{
		std::cerr << "No http block in config: nothing to serve." << std::endl;
		return false;
	}

	// Already deduplicated by address and port, and canonicalised, by the
	// config module.
	const std::vector<std::string> addressPortPairs =
		_mainConfig.getHttpConfig().getServerConfigAddressPortPairs();

	for (size_t i = 0; i < addressPortPairs.size(); ++i)
	{
		const std::string &addressPortPair = addressPortPairs[i];
		const std::pair<std::string, uint16_t> addressAndPort =
			IpAddressPortPairHelpers::splitCanonicalIpAddressPortPair(
				addressPortPair);

		int fd = createListener(addressAndPort.first, addressAndPort.second);
		if (fd < 0)
		{
			return false;
		}

		pollfd lpfd;
		lpfd.fd = fd;
		lpfd.events = POLLIN;
		lpfd.revents = 0;

		_listeners.push_back(fd);
		_listenerAddressPortPairs[fd] = addressPortPair;
		_pfds.push_back(lpfd);

		std::cout << "Listening on " << addressPortPair
				  << std::endl; // TODO: Remove debug message
	}
	return !_listeners.empty();
}

void Server::runServer()
{
	for (;;)
	{
		int nready = poll(&_pfds[0], static_cast<nfds_t>(_pfds.size()), -1);
		if (nready < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			perror("poll");
			break;
		}
		for (size_t i = 0; i < _pfds.size() && nready > 0;)
		{
			if (_pfds[i].revents == 0)
			{
				++i;
				continue;
			}
			--nready;
			int fd = _pfds[i].fd;
			if (_listenerAddressPortPairs.count(fd))
			{
				acceptClient(fd);
				++i;
			}
			else if (clientEventHandler(i))
			{
				++i;
			}
			// else: removeClient shifted the next pfd into position i
		}
	}
}

bool Server::clientEventHandler(size_t i)
{
	short rev = _pfds[i].revents;
	int fd = _pfds[i].fd;
	Client &cl = _clients[fd];

	if (!cl.closeFlag() && (rev & POLLIN))
	{
		cl.onRecv(fd);
	}
	if (rev & POLLOUT)
	{
		cl.onSend(fd);
	}
	// A client that wants to close may still have a response queued, so hold
	// the connection open until everything buffered has been flushed.
	if ((rev & (POLLERR | POLLHUP)) || (cl.closeFlag() && !cl.writeFlag()))
	{
		removeClient(i);
		return false;
	}
	syncEvents(i, cl);
	return true;
}

void Server::acceptClient(int listener_fd)
{
	sockaddr_storage client_addr;
	socklen_t client_len;
	pollfd pfd;
	int client_fd;
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];

	client_len = sizeof(client_addr);
	client_fd = accept(listener_fd, reinterpret_cast<sockaddr *>(&client_addr),
	                   &client_len);
	if (client_fd < 0)
	{
		return;
	}
	if (setNonblock(client_fd) < 0)
	{
		perror("fcntl");
		close(client_fd);
		return;
	}
	_clients[client_fd] = Client();
	_clients[client_fd].setAddressPortPair(
		_listenerAddressPortPairs[listener_fd]);

	pfd.fd = client_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pfds.push_back(pfd);

	if (getnameinfo(reinterpret_cast<sockaddr *>(&client_addr), client_len,
	                host, sizeof(host), svc, sizeof(svc),
	                NI_NUMERICHOST | NI_NUMERICSERV) == 0)
	{
		std::cout << "Connected fd= " << client_fd << " from " << host << ":"
				  << svc << std::endl;
	}
}

void Server::removeClient(size_t i)
{
	int fd = _pfds[i].fd;

	std::cout << "Removing fd=" << fd << std::endl;
	close(fd);
	_clients.erase(fd);
	_pfds.erase(_pfds.begin() + i);
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
void Server::syncEvents(size_t i, const Client &client)
{
	_pfds[i].events = POLLIN;
	if (client.writeFlag())
	{
		_pfds[i].events |= POLLOUT;
	}
}

int Server::setNonblock(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
	{
		return -1;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int Server::createListener(const std::string &host, int port)
{
	char portStr[16];
	int fd;
	int opt = 1;
	int err;
	addrinfo hints;
	addrinfo *res;

	std::snprintf(portStr, sizeof(portStr), "%d", port);

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

	err = getaddrinfo(hostArg, portStr, &hints, &res);
	if (err != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(err) << std::endl;
		return -1;
	}

	fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd < 0)
	{
		perror("socket");
		freeaddrinfo(res);
		return -1;
	}
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (res->ai_family == AF_INET6)
	{
		// Otherwise a wildcard IPv6 listener also claims the IPv4 port, and a
		// separately configured IPv4 listener on that port fails to bind.
		setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
	}
	if (bind(fd, res->ai_addr, res->ai_addrlen) < 0)
	{
		perror("bind");
		close(fd);
		freeaddrinfo(res);
		return -1;
	}
	freeaddrinfo(res);

	if (listen(fd, 1024) < 0) // TODO: Magic number
	{
		perror("listen");
		close(fd);
		return -1;
	}
	if (setNonblock(fd) < 0)
	{
		perror("fcntl");
		close(fd);
		return -1;
	}
	return fd;
}
