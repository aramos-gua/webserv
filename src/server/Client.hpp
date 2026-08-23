#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "ServerConfig.hpp"
#include "../RequestParser.hpp"
#include "../HttpResponse.hpp"
#include "../HttpResponseBuilder.hpp"

class Client {
	public:
		Client();
		Client(const Client& copy);
		Client &operator=(const Client& copy);
		~Client();

		bool	writeFlag()const;
		bool	closeFlag()const;

		void	setConfig(const ServerConfig *cfg);

		void	onRecv(int fd);
		void	onSend(int fd);

	private:
		std::string			_recv_buf;
		std::string			_send_buf;
		bool				_close;
		const ServerConfig	*_cfg;

		//TODO: Fill with HTTP parsing
		// buf -> all bytes received so far - erase what is used
		//return full response to send, or "" to wait for more data
		RequestParser	_parser;
		std::string handle(std::string &buf);
};

#endif
