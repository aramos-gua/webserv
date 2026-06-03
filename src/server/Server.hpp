#ifndef SERVER_HPP
#define SERVER_HPP

#include "ServerConfig.hpp"
#include "Client.hpp"

#include <map>
#include <vector>
#include <poll.h>

class Server {
	public:
		Server(const ServerConfig &cfg);
		~Server();

		bool	start();
		void	run();

	private:
		ServerConfig			_cfg;
		int						_listener;
		std::vector<pollfd>		_pfds;
		std::map<int, Client>	_clients;

		bool					dispatch(size_t i);
		void					acceptClient();
		void					removeClient(int fd);
		void					syncEvents(int fd);

		static int				setNonblock(int fd);
		static int				makeListener(int fd);

		Server(const Server&);
		Server &operator=(const Server&);
};

#endif
