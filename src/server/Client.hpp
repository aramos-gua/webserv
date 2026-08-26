/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:40:06 by manwar            #+#    #+#             */
/*   Updated: 2026/08/26 02:10:31 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include "HttpResponseBuilder.hpp"

class HttpConfig;

class Client
{
public:
	Client();
	Client(const Client &copy);
	Client &operator=(const Client &copy);
	~Client();

	bool writeFlag() const;
	bool closeFlag() const;

	void setUp(const HttpConfig &httpConfig,
	           const std::string &addressPortPair);

	void onRecv(int fd);
	void onSend(int fd);

private:
	std::string _send_buf;
	bool _close;
	const HttpConfig *_httpConfig;
	std::string _addressPortPair;
	HttpRequestParser _parser;

	void handleRequest();
	// TODO: Replace with real request handling, driven by the resolved config
	void queuePlainTextResponse(int statusCode, const std::string &description,
	                            const std::string &body);

	static std::string getServerNameFromHostHeader(
		const std::string &hostHeader);
};

#endif
