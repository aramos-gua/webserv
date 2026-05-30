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

#include "RequestParser.hpp"
#include <iostream>

int	main()
{
	std::string	rawRequest =
		"POST /wordpress/wp-admin HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: Mozilla\r\n"
        "Content-Length: 11\r\n"
        "\r\n"
        "hello world";
	    try
    {
        HttpRequest req = RequestParser::parse(rawRequest);

        std::cout << "METHOD: " << req.method << std::endl;
        std::cout << "PATH: " << req.path << std::endl;
        std::cout << "VERSION: " << req.version << std::endl;

        std::cout << std::endl;
        std::cout << "HEADERS:" << std::endl;

        std::map<std::string, std::string>::iterator it;

        for (it = req.headers.begin();
             it != req.headers.end();
             ++it)
        {
            std::cout << it->first
                      << " => "
                      << it->second
                      << std::endl;
        }

        std::cout << std::endl;
        std::cout << "BODY:" << std::endl;
        std::cout << req.body << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
