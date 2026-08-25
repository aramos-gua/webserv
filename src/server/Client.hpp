/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:40:06 by manwar            #+#    #+#             */
/*   Updated: 2026/08/25 21:58:36 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include "HttpResponseBuilder.hpp"

class Client
{
public:
	Client();
	Client(const Client &copy);
	Client &operator=(const Client &copy);
	~Client();

	bool writeFlag() const;
	bool closeFlag() const;

	void setAddressPortPair(const std::string &addressPortPair);

	void onRecv(int fd);
	void onSend(int fd);

private:
	std::string _send_buf;
	bool _close;
	// The listener this client arrived on; the server block itself is resolved
	// per request, from this pair plus the request's Host header.
	std::string _addressPortPair;
	HttpRequestParser _parser;

	// TODO: Replace with real request handling, driven by the resolved config
	void queuePlainTextResponse(int statusCode, const std::string &description,
	                            const std::string &body);
};

#endif
