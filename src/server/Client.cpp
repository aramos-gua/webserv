/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:39:35 by manwar            #+#    #+#             */
/*   Updated: 2026/08/25 21:58:07 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>

#include "Client.hpp"

static const int BUFSIZE = 2048; // TODO: Change?

Client::Client(): _send_buf(), _close(false), _addressPortPair(), _parser()
{
}

Client::Client(const Client &copy)
	: _send_buf(copy._send_buf), _close(copy._close),
	  _addressPortPair(copy._addressPortPair), _parser(copy._parser)
{
}

Client::~Client()
{
}

Client &Client::operator=(const Client &copy)
{
	if (this != &copy)
	{
		this->_send_buf = copy._send_buf;
		this->_close = copy._close;
		this->_addressPortPair = copy._addressPortPair;
		this->_parser = copy._parser;
	}
	return *this;
}

void Client::setAddressPortPair(const std::string &addressPortPair)
{
	_addressPortPair = addressPortPair;
}

bool Client::writeFlag() const
{
	return !_send_buf.empty();
}

bool Client::closeFlag() const
{
	return _close;
}

void Client::onRecv(int fd)
{
	char tmp[BUFSIZE];
	ssize_t bytes;

	bytes = recv(fd, tmp, sizeof(tmp), 0);
	if (bytes <= 0)
	{
		_close = true;
		return;
	}
	HttpRequestParser::Result res =
		_parser.feed(tmp, static_cast<size_t>(bytes));
	if (res == HttpRequestParser::COMPLETE)
	{
		queuePlainTextResponse(200, "OK", "Webserv is working\n");
		_parser.reset();
	}
	else if (res == HttpRequestParser::ERROR)
	{
		queuePlainTextResponse(400, "Bad Request", "Malformed request\n");
		_close = true;
	}
}

void Client::queuePlainTextResponse(int statusCode,
                                    const std::string &description,
                                    const std::string &body)
{
	HttpResponse response;

	response.statusCode = statusCode;
	response.description = description;
	response.headers["Content-Type"] = "text/plain";
	response.body = body;
	_send_buf += HttpResponseBuilder::build(response);
}

void Client::onSend(int fd)
{
	ssize_t bytes;

	bytes = send(fd, _send_buf.data(), _send_buf.size(), 0);
	if (bytes < 0)
	{
		_close = true;
	}
	else if (bytes > 0)
	{
		_send_buf.erase(0, static_cast<size_t>(bytes));
	}
}
