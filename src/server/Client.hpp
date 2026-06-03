#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
	public:
		Client();
		Client(const Client& copy);
		Client &operator=(const Client& copy);
		~Client();

		bool	writeFlag()const;
		bool	closeFlag()const;

		void	onReadable(int fd);
		void	onWritable(int fd);

	private:
		std::string	_recv_buf;
		std::string	_send_buf;
		bool		_close;

		//TODO: Fill with HTTP parsing
		//buf -> all bytes received so far - erase what is used
		//return full response to send, or "" to wait for more data
		std::string handle(std::string &buf);
};

#endif
