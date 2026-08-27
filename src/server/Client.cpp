/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:39:35 by manwar            #+#    #+#             */
/*   Updated: 2026/08/27 15:46:35 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <map>
#include <stdexcept>
#include <sys/socket.h>

#include "Client.hpp"
#include "HttpConfig.hpp"
#include "HttpStatusCodeHelpers.hpp"
#include "ServerConfig.hpp"
#include "StringBase.hpp"
#include "StringHelpers.hpp"

static const int BUFSIZE = 2048; // TODO: Change?

Client::Client(void): shouldClose(false), httpConfig(NULL)
{
}

Client::Client(const Client &copy)
	: sendBuffer(copy.sendBuffer), shouldClose(copy.shouldClose),
	  httpConfig(copy.httpConfig), addressPortPair(copy.addressPortPair),
	  requestParser(copy.requestParser)
{
}

Client::~Client(void)
{
}

Client &Client::operator=(const Client &copy)
{
	if (this != &copy)
	{
		this->sendBuffer = copy.sendBuffer;
		this->shouldClose = copy.shouldClose;
		this->httpConfig = copy.httpConfig;
		this->addressPortPair = copy.addressPortPair;
		this->requestParser = copy.requestParser;
	}
	return *this;
}

void Client::setUp(const HttpConfig &httpConfig,
                   const std::string &addressPortPair)
{
	this->httpConfig = &httpConfig;
	this->addressPortPair = addressPortPair;
}

bool Client::getWhetherOutputIsPending(void) const
{
	return !sendBuffer.empty();
}

bool Client::getWhetherConnectionShouldClose(void) const
{
	return shouldClose;
}

void Client::onRecv(int fileDescriptor)
{
	char buffer[BUFSIZE];
	ssize_t bytesRead;

	bytesRead = recv(fileDescriptor, buffer, sizeof(buffer), 0);
	if (bytesRead <= 0)
	{
		shouldClose = true;
		return;
	}
	HttpRequestParser::Result result =
		requestParser.feed(buffer, static_cast<std::size_t>(bytesRead));
	if (result == HttpRequestParser::COMPLETE)
	{
		handleRequest();
		requestParser.resetInPreparationForNextRequest();
	}
	else if (result == HttpRequestParser::INVALID)
	{
		queuePlainTextResponse(requestParser.getErrorStatusCode(),
		                       "Malformed request\n");
		shouldClose = true;
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

void Client::handleRequest(void)
{
	const HttpRequest &request = requestParser.getRequest();
	std::string hostHeader;

	std::map<std::string, std::string>::const_iterator hostIterator =
		request.headers.find("host");
	if (hostIterator != request.headers.end())
	{
		hostHeader = hostIterator->second;
	}
	const std::string serverName = getServerNameFromHostHeader(hostHeader);

	if (httpConfig == NULL)
	{
		queuePlainTextResponse(INTERNAL_SERVER_ERROR,
		                       "Client has no configuration\n");
		shouldClose = true;
		return;
	}
	try
	{
		const ServerConfig &serverConfig =
			httpConfig->resolveServerConfigSettingForRequestTarget(
				addressPortPair, serverName);

		// TODO: Replace with real request handling, driven by serverConfig
		queuePlainTextResponse(
			OK, StringBase()
					<< "Webserv is working\n\n"
					<< "listener:    " << addressPortPair << "\n"
					<< "host header: "
					<< (hostHeader.empty() ? "(none)" : hostHeader) << "\n"
					<< "server name: "
					<< (serverName.empty() ? "(none)" : serverName) << "\n"
					<< "root:        "
					<< (serverConfig.rootSettingResolves()
		                    ? serverConfig.resolveRootSetting()
		                    : "(not set)")
					<< "\n");
	}
	catch (const AConfig::SettingNotSetException &exception)
	{
		// The listener was bound from this same set of pairs, so this should be
		// unreachable; report it rather than dropping the connection silently.
		queuePlainTextResponse(INTERNAL_SERVER_ERROR,
		                       StringBase() << exception.what() << "\n");
		shouldClose = true;
	}
}

void Client::queuePlainTextResponse(HttpStatusCode statusCode,
                                    const std::string &body)
{
	HttpResponse response;

	response.statusCode = statusCode;
	response.description =
		HttpStatusCodeHelpers::getReasonPhraseForStatusCode(statusCode);
	response.headers["Content-Type"] = "text/plain";
	response.body = body;
	sendBuffer += HttpResponseBuilder::build(response);
}

void Client::onSend(int fileDescriptor)
{
	ssize_t bytesSent;

	bytesSent = send(fileDescriptor, sendBuffer.data(), sendBuffer.size(), 0);
	if (bytesSent < 0)
	{
		shouldClose = true;
	}
	else if (bytesSent > 0)
	{
		sendBuffer.erase(0, static_cast<std::size_t>(bytesSent));
	}
}
