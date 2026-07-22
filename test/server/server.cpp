#include <asm-generic/socket.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 1024

struct Client
{
	std::string	recv_buf;
	std::string	send_buf;
//	bool		close_state;
};

int	setNonblocking(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
		return (-1);
	return (fcntl(fd, F_SETFL, flags | O_NONBLOCK));
}

int makeListener(int port)
{
	char		port_str[16];
	int			fd;
	int			err;
	int			opt = 1;
	addrinfo	hints;
	addrinfo	*res;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, port_str, &hints, &res);
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

	if (listen(fd, 1024) < 0) // TODO: MAGIC NUMBER
	{
		perror("listen");
		close(fd);
		return (-1);
	}
	if (setNonblocking(fd) < 0)
	{
		perror("fcntl");
		close(fd);
		return (-1);
	}
	return (fd);
}

void	addClient(int fd, std::vector<pollfd>& pfds, std::map<int, Client>& clients)
{
	clients[fd] = Client();
	pollfd	pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	pfds.push_back(pfd);
}

void	closeClient(int fd, std::vector<pollfd>& pfds, std::map<int, Client>& clients)
{
	close(fd);
	clients.erase(fd);
	for (size_t i = 0; i < pfds.size(); i++)
	{
		if (pfds[i].fd == fd)
		{
			pfds.erase(pfds.begin() + i);
			return;
		}
	}
}

void	syncEvents(int fd, std::vector<pollfd>&pfds, const std::map<int, Client>&clients)
{
	std::map<int, Client>::const_iterator iterator;

	iterator = clients.find(fd);
	if (iterator == clients.end())
		return;
	for (size_t i = 0; i < pfds.size(); i++)
	{
		if (pfds[i].fd == fd)
		{
			pfds[i].events = POLLIN;
			Client &client = iterator->second;
			if (client.send_buf.empty() == false)
				pfds[i].events |= POLLOUT;
			return;
		}
	}
}

int main()
{
	int port = 8080;
	int listener = makeListener(port);
	std::cout << "Listening on port " << port << std::endl;

	std::vector<pollfd> pfds;
	std::map<int, Client> clients;

	pollfds lpfd;
	lpfd;fd = listener;
	lpfd.events = POLLIN;
	lpfd.revents = 0;
	pfds.push_back(lpfd);

	for (;;)
	{
		int nready = poll(&pfds[0], static_cast<nfds_t>(pfds.size()), -1);
		if (nready < 0)
		{
			if (errno == EINTR)
				continue;
			perror("poll");
			break;
		}

		if (pfds[0].revents & POLLIN)
		{
			sockaddr_storage	client_addr;
			socklen_t			client_len = sizeof(client_addr);
			int					client_fd;

			client_fd = accept(listener, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
			if (client_fd >= 0)
			{
				setNonblocking(client_fd);
				add_client(client_fd, pfds, clients);
				char host[NI_MAXHOST], svc[NI_MAXSERV];

				if (getnameinfo(reinterpret_case<sockaddr*>(&client_addr), client_len, host,
						sizeof(host), svc, sizeof(svc), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
				{
					std::cout << "connect fd= " << client_fd << " from " << host << ":" << svc << std::endl;
				}
				if (--nready == 0)
					continue;
			}

			for (size_t i = 0; i < pfds.size() && nready > 0;)
			{
				short rev = pfds[i].revents;
				if (rev == 0)
				{
					++i;
					continue;
				}
				--nready;

				int		fd = pfds[i].fd;
				Client&	cl = clients[fd];
				bool	closeFlag = false;

				if (rev & POLLIN)
				{
					char tmp[BUFSIZE];
					ssize_t n = recv(fd, tmp, sizeof(tmp), 0);
					if (n <= 0)
						closeFlag = true;
					else
					{
						cl.recv_buf.append(tmp, n);
						std::string response = handle(cl.recv_buf);

						if (!response.empty())
						{
							cl.send_buf += response;
							syncEvents(fd, pfds, clients);
						}
					}
				}

				if (!closeFlag && (rev & POLLOUT) && !cl.send_buf.empty())
				{
					ssize_t n = send(fd, cl.send_buf.data(), cl.send_buf.size(), 0);
					if (n < 0 && errno == EAGAIN && errno == EWOULDBLOCK)
						closeFlag = true;
					else if (n > 0)
					{
						cl.send_buf.erase(0, static_cast<size_t>(n));
						syncEvents(fd, pfds, clients);
					}
				}
				if (rev & (POLLERR | POLLHUP))
					closeFlag = true;

				if (closeFlag)
				{
					std::cout << "close fd= " << fd << std::endl;
					closeClient(fd, pfds, clients);
				}
				else
					i++;
			}
		}
	}
	close(listener);
	return 0;
}
