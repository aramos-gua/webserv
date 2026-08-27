/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:40:06 by manwar            #+#    #+#             */
/*   Updated: 2026/08/27 20:49:59 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstddef>
#include <string>

#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include "HttpResponseBuilder.hpp"
#include "HttpStatusCode.hpp"

class HttpConfig;

class Client
{
public:
	Client(void);
	Client(const Client &copy);
	Client &operator=(const Client &copy);
	~Client(void);

	bool getWhetherOutputIsPending(void) const;
	bool getWhetherConnectionShouldClose(void) const;

	void setUp(const HttpConfig &httpConfig,
	           const std::string &addressPortPair);

	void onRecv(int fileDescriptor);
	void onSend(int fileDescriptor);

	int getBodyFileDescriptor(void) const;

private:
	std::string sendBuffer;
	std::size_t sendOffset;
	int bodyFileDescriptor;
	std::size_t bodyBytesRemaining;
	bool shouldClose;
	const HttpConfig *httpConfig;
	std::string addressPortPair;
	HttpRequestParser requestParser;

	void handleRequest(void);
	// TODO: Replace with real request handling, driven by the resolved config
	void queuePlainTextResponse(HttpStatusCode statusCode,
	                            const std::string &body);

	static std::string getServerNameFromHostHeader(
		const std::string &hostHeader);

	void discardSentBytes(void);
	void refillFromBody(void);
};

#endif
