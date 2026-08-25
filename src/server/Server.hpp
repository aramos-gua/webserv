/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manwar <manwar@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:40:11 by manwar            #+#    #+#             */
/*   Updated: 2026/08/25 21:40:12 by manwar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <poll.h>
#include <string>
#include <vector>

#include "Client.hpp"
#include "ServerConfig.hpp"

class Server
{
public:
	Server(const std::vector<ServerConfig> &cfgs);
	~Server();

	bool startServer();
	void runServer();

private:
	std::vector<ServerConfig> _cfgs;
	std::vector<int> _listeners;
	std::map<int, const ServerConfig *> _listenerFds;
	std::vector<pollfd> _pfds;
	std::map<int, Client> _clients;

	bool clientEventHandler(size_t i);
	void acceptClient(int listener_fd);
	void removeClient(size_t i);
	void syncEvents(size_t i, const Client &client);

	static int setNonblock(int fd);
	static int createListener(const std::string &host, int port);

	Server(const Server &);
	Server &operator=(const Server &);
};

#endif
