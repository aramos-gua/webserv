/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:35:00 by manwar            #+#    #+#             */
/*   Updated: 2026/08/25 23:01:26 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <exception>
#include <iostream>

#include "ConfigParser.hpp"
#include "MainConfig.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <path to config file>"
				  << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		MainConfig mainConfig;
		ConfigParser(argv[1], mainConfig);
		mainConfig.freeze(argv[1]);

		Server server(mainConfig);
		if (!server.startServer())
		{
			std::cerr << "Failed to start server." << std::endl;
			return EXIT_FAILURE;
		}
		server.runServer();
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
