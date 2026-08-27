/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:58:38 by aramos            #+#    #+#             */
/*   Updated: 2026/08/25 21:58:28 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "HttpRequestParser.hpp"

int main(void)
{
	try
	{
		std::string rawRequest = "POST /wordpress/wp-admin HTTP/1.0\r\n"
								 "Host: localhost:8080\r\n"
								 "User-Agent: Mozilla\r\n"
								 "Content-Length: 11\r\n"
								 "\r\n"
								 "hello world";

		HttpRequestParser parser;

		// simulate a fragmented TCP read: feed it in two pieces
		std::size_t mid = rawRequest.size() / 2;
		parser.feed(rawRequest.c_str(), mid);
		HttpRequestParser::Result result =
			parser.feed(rawRequest.c_str() + mid, rawRequest.size() - mid);

		if (result != HttpRequestParser::COMPLETE)
		{
			std::cerr << "Parse failed or incomplete: " << parser.getError()
					  << std::endl;
			return EXIT_FAILURE;
		}

		const HttpRequest &request = parser.getRequest();
		std::cout << "METHOD: " << request.method << std::endl;
		std::cout << "PATH: " << request.path << std::endl;
		std::cout << "VERSION: " << request.version << std::endl;

		std::cout << "\nHEADERS:\n";
		std::map<std::string, std::string>::const_iterator headerIterator;
		for (headerIterator = request.headers.begin();
		     headerIterator != request.headers.end(); ++headerIterator)
		{
			std::cout << headerIterator->first << " => "
					  << headerIterator->second << std::endl;
		}

		std::cout << "\nBODY:\n" << request.body << std::endl;
	}
	catch (const std::exception &exception)
	{
		std::cerr << "An error occurred during execution:" << std::endl;
		std::cerr << exception.what() << "." << std::endl;
		std::cerr << "Unable to continue." << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
