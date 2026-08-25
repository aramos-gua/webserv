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
#include "ServerConfig.hpp"

class Client
{
public:
	Client();
	Client(const Client &copy);
	Client &operator=(const Client &copy);
	~Client();

	bool writeFlag() const;
	bool closeFlag() const;

	void setConfig(const ServerConfig *cfg);

	void onRecv(int fd);
	void onSend(int fd);

private:
	std::string _send_buf;
	bool _close;
	const ServerConfig *_cfg;
	HttpRequestParser _parser;

	// TODO: Replace with real request handling, driven by _cfg
	void queuePlainTextResponse(int statusCode, const std::string &description,
	                            const std::string &body);
};

#endif
