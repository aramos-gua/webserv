#include "Client.hpp"

#include <cerrno>
#include <sys/socket.h>

static const int BUFSIZE = 2048;	//TODO: Change?

Client::Client() : _close(false)
{
}

Client::Client(const Client &copy) : _recv_buf(copy._recv_buf), _send_buf(copy._send_buf), _close(copy._close)
{
}

Client::~Client()
{
}

Client &Client::operator=(const Client &copy)
{
	if (this != &copy)
	{
		this->_recv_buf = copy._recv_buf;
		this->_send_buf = copy._send_buf;
		this->_close = copy._close;
	}
	return (*this);
}

bool	Client::writeFlag() const
{
	return (!_send_buf.empty());
}

bool	Client::closeFlag() const
{
	return (_close);
}

void	Client::onReadable(int fd)
{
	char		tmp[BUFSIZE];
	std::string resp;
	ssize_t		bytes;

	bytes = recv(fd, tmp, sizeof(tmp), 0);
	if (bytes < 0)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			_close = true;
		return;
	}
	if (bytes == 0)
	{
		_close = true;
		return;
	}
	_recv_buf.append(tmp, bytes);
	resp = handle(_recv_buf);
	if (!resp.empty())
		_send_buf += resp;
}

std::string	Client::handle(std::string &buf)
{
	(void)buf;
	//TODO: Fill with HTTP parsing
	return ("");
}

void	Client::onWritable(int fd)
{
	ssize_t	bytes;

	bytes = send(fd, _send_buf.data(), _send_buf.size(), 0);
	if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)		//TODO: not allowed?
		_close = true;
	else if (bytes > 0)
		_send_buf.erase(0, static_cast<size_t>(bytes));
}
