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

#include <cstddef>
#include <map>
#include <poll.h>
#include <string>
#include <vector>

#include "Client.hpp"
#include "MainConfig.hpp"

class Server
{
public:
	Server(const MainConfig &mainConfig);
	~Server(void);

	bool startServer(void);
	void runServer(void);

private:
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
	const MainConfig &mainConfig;
	std::vector<int> listenerFileDescriptors;
	std::map<int, std::string> listenerAddressPortPairs;
	std::vector<pollfd> pollFds;
	std::map<int, Client> clients;

	bool clientEventHandler(std::size_t pollFdIndex);
	void acceptClient(int listenerFileDescriptor);
	void removeClient(std::size_t pollFdIndex);
	void syncEvents(std::size_t pollFdIndex, const Client &client);

	static int setNonblock(int fileDescriptor);
	static int createListener(const std::string &host, int port);

	Server(const Server &);
	Server &operator=(const Server &);
};

#endif
