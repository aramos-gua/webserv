/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:39:35 by manwar            #+#    #+#             */
/*   Updated: 2026/08/26 02:07:27 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <map>
#include <stdexcept>
#include <sys/socket.h>

#include "Client.hpp"
#include "HttpConfig.hpp"
#include "ServerConfig.hpp"
#include "StringBase.hpp"
#include "StringHelpers.hpp"

static const int BUFSIZE = 2048; // TODO: Change?

Client::Client()
	: _send_buf(), _close(false), _httpConfig(NULL), _addressPortPair(),
	  _parser()
{
}

Client::Client(const Client &copy)
	: _send_buf(copy._send_buf), _close(copy._close),
	  _httpConfig(copy._httpConfig), _addressPortPair(copy._addressPortPair),
	  _parser(copy._parser)
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
		this->_httpConfig = copy._httpConfig;
		this->_addressPortPair = copy._addressPortPair;
		this->_parser = copy._parser;
	}
	return *this;
}

void Client::setUp(const HttpConfig &httpConfig,
                   const std::string &addressPortPair)
{
	_httpConfig = &httpConfig;
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
		handleRequest();
		_parser.reset();
	}
	else if (res == HttpRequestParser::ERROR)
	{
		queuePlainTextResponse(400, "Bad Request", "Malformed request\n");
		_close = true;
	}
}

std::string Client::getServerNameFromHostHeader(const std::string &hostHeader)
{
	std::string serverName = hostHeader;
	std::size_t portSeparatorIndex;

	if (!serverName.empty() && serverName[0] == '[')
	{
		std::size_t closingBracket = serverName.find(']');
		portSeparatorIndex = closingBracket == std::string::npos
		                         ? std::string::npos
		                         : serverName.find(':', closingBracket);
	}
	else
	{
		portSeparatorIndex = serverName.find(':');
	}
	if (portSeparatorIndex != std::string::npos)
	{
		serverName.erase(portSeparatorIndex);
	}
	return StringHelpers::toLowercase(serverName);
}

void Client::handleRequest()
{
	const HttpRequest &request = _parser.getRequest();
	std::string hostHeader;

	std::map<std::string, std::string>::const_iterator hostIterator =
		request.headers.find("host");
	if (hostIterator != request.headers.end())
	{
		hostHeader = hostIterator->second;
	}
	const std::string serverName = getServerNameFromHostHeader(hostHeader);

	if (_httpConfig == NULL)
	{
		queuePlainTextResponse(500, "Internal Server Error",
		                       "Client has no configuration\n");
		_close = true;
		return;
	}
	try
	{
		const ServerConfig &serverConfig =
			_httpConfig->getServerConfig(_addressPortPair, serverName);

		// TODO: Replace with real request handling, driven by serverConfig
		queuePlainTextResponse(
			200, "OK",
			StringBase() << "Webserv is working\n\n"
						 << "listener:    " << _addressPortPair << "\n"
						 << "host header: "
						 << (hostHeader.empty() ? "(none)" : hostHeader) << "\n"
						 << "server name: "
						 << (serverName.empty() ? "(none)" : serverName) << "\n"
						 << "root:        "
						 << (serverConfig.getWhetherRootSet()
		                         ? serverConfig.getRoot()
		                         : "(not set)")
						 << "\n");
	}
	catch (const std::out_of_range &exception)
	{
		// The listener was bound from this same set of pairs, so this should be
		// unreachable; report it rather than dropping the connection silently.
		queuePlainTextResponse(500, "Internal Server Error",
		                       StringBase() << exception.what() << "\n");
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
