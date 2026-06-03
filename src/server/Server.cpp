#include "Server.hpp"

#include <cerrno>
#include <iostream>
#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(const ServerConfig &cfg) : _cfg(cfg), _listener(-1)
{

}

Server::~Server()
{
	if (_listener >= 0)
		close(_listener);
}

bool	Server::startServer()
{
	pollfd	lpfd;

	_listener = createListener(_cfg.port);
	if (_listener < 0)
		return (false);
	lpfd.fd = _listener;
	lpfd.events = POLLIN;
	lpfd.revents = 0;
	_pfds.push_back(lpfd);

	std::cout << "Listening on port: " << _cfg.port << std::endl;		//TODO: Remove debug message
	return (true);
}

void	Server::runServer()
{
	for (;;)
	{
		int	nready = poll(&_pfds[0], static_cast<nfds_t>(_pfds.size()), -1);
		if (nready < 0)
		{
			if (errno == EINTR)												//TODO: not allowed?
				continue;
			perror("poll");
			break;
		}
		if (_pfds[0].revents & POLLIN)
		{
			acceptClient();
			if (--nready == 0)
				continue;
		}
		for (size_t i = 1; i < _pfds.size() && nready > 0;)
		{
			if (_pfds[i].revents == 0)
			{
				++i;
				continue;
			}
			--nready;
			if (clientEventHandler(i))
				++i;
			else
				continue;					// remove client, next client position into position i
		}
	}
}

bool	Server::clientEventHandler(size_t i)
{
	short	rev = _pfds[i].revents;
	int		fd = _pfds[i].fd;
	Client	&cl = _clients[fd];

	if (rev & POLLIN)
		cl.onReadable(fd);
	if (!cl.closeFlag() && (rev & POLLOUT))
		cl.onWritable(fd);
	if ((rev & (POLLERR | POLLHUP)) || cl.closeFlag())
	{
		removeClient(fd);
		return (false);
	}
	syncEvents(fd);
	return (true);
}

void	Server::acceptClient()
{
	sockaddr_storage	client_addr;
	socklen_t			client_len;
	pollfd				pfd;
	int					client_fd;
	char				host[NI_MAXHOST];
	char				svc[NI_MAXSERV];

	client_len = sizeof(client_addr);
	client_fd = accept(_listener, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
	if (client_fd < 0)
		return;
	setNonblock(client_fd);
	_clients[client_fd] = Client();

	pfd.fd = client_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pfds.push_back(pfd);

	if (getnameinfo(reinterpret_cast<sockaddr*>(&client_addr), client_len,
				host, sizeof(host), svc, sizeof(svc),
				NI_NUMERICHOST | NI_NUMERICSERV) == 0)
		std::cout << "Connected fd= " << client_fd << " from " << host << ":" << svc << std::endl;
}

void	Server::removeClient(int fd)
{
	std::cout << "Removing fd=" << fd << std::endl;
	close(fd);
	_clients.erase(fd);

	for (size_t i = 0; i < _pfds.size(); ++i)
	{
		if (_pfds[i].fd == fd)
		{
			_pfds.erase(_pfds.begin() + i);
			return;
		}
	}
}

void	Server::syncEvents(int fd)
{
	for (size_t i = 0; i < _pfds.size; ++i)
	{
		if (_pfds[i].fd == fd)
		{
			_pfds[i].events = POLLIN;
			if (_clients[fd].writeFlag()) _pfds[i].events |= POLLOUT;
			return;
		}
	}
}

int	Server::setNonblock(int fd)
{
	int	flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
		return (-1);
	return (fcntl(fd, F_SETFL, flags | O_NONBLOCK));
}

int		Server::createListener(int port)
{
	char		portStr[16];
	int			fd;
	int			opt = 1;
	int			err;
	addrinfo	hints;
	addrinfo	*res;

	std::snprintf(portStr, sizeof(portStr), "%d", port);

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	err = getaddrinfo(NULL, portStr, &hints, &res);
	if (err != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(err) << std::endl;
		return (-1);
	}

	fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd < 0)
	{
		perror("socket");
		freeaddrinfo(res);
		return (-1);
	}
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(fd, res->ai_addr, res->ai_addrlen) < 0)
	{
		perror("bind");
		close(fd);
		freeaddrinfo(res);
		return (-1);
	}
	freeaddrinfo(res);

	if (listen(fd, 1024) < 0)	//TODO: Magic number
	{
		perror("listen");
		close(fd);
		return (-1);
	}
	if (setNonblock(fd) < 0)
	{
		perror("fcntl");
		close(fd);
		return (-1);
	}
	return (fd);


}
