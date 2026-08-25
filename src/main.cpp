/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aramos <contact@aramos.dev>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:58:38 by aramos            #+#    #+#             */
/*   Updated: 2026/05/30 13:00:13 by aramos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "RequestParser.hpp"

int main()
{
	std::string rawRequest = "POST /wordpress/wp-admin HTTP/1.0\r\n"
							 "Host: localhost:8080\r\n"
							 "User-Agent: Mozilla\r\n"
							 "Content-Length: 11\r\n"
							 "\r\n"
							 "hello world";

	RequestParser parser;

	// simulate a fragmented TCP read: feed it in two pieces
	size_t mid = rawRequest.size() / 2;
	parser.feed(rawRequest.c_str(), mid);
	RequestParser::Result result =
		parser.feed(rawRequest.c_str() + mid, rawRequest.size() - mid);

	if (result != RequestParser::COMPLETE)
	{
		std::cerr << "Parse failed or incomplete: " << parser.getError()
				  << std::endl;
		return 1;
	}

	const HttpRequest &req = parser.getRequest();
	std::cout << "METHOD: " << req.method << std::endl;
	std::cout << "PATH: " << req.path << std::endl;
	std::cout << "VERSION: " << req.version << std::endl;

	std::cout << "\nHEADERS:\n";
	std::map<std::string, std::string>::const_iterator it;
	for (it = req.headers.begin(); it != req.headers.end(); ++it)
	{
		std::cout << it->first << " => " << it->second << std::endl;
	}

	std::cout << "\nBODY:\n" << req.body << std::endl;
	return 0;
}
