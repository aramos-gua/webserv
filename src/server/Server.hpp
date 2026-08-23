#ifndef SERVER_HPP
#define SERVER_HPP

#include "ServerConfig.hpp"
#include "Client.hpp"

#include <map>
#include <string>
#include <vector>
#include <poll.h>

class Server {
	public:
		Server(const std::vector<ServerConfig> &cfgs);
		~Server();

		bool	startServer();
		void	runServer();

	private:
		std::vector<ServerConfig>			_cfgs;
		std::vector<int>					_listeners;
		std::map<int, const ServerConfig*>	_listenerFds;
		std::vector<pollfd>					_pfds;
		std::map<int, Client>				_clients;

		bool					clientEventHandler(size_t i);
		void					acceptClient(int listener_fd);
		void					removeClient(int fd);
		void					syncEvents(int fd);

		static int				setNonblock(int fd);
		static int				createListener(const std::string &host, int port);

		Server(const Server&);
		Server &operator=(const Server&);
};

#endif
