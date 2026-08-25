/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manwar <manwar@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 22:17:56 by manwar            #+#    #+#             */
/*   Updated: 2026/08/25 22:17:57 by manwar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <vector>

#include "Server.hpp"
#include "ServerConfig.hpp"

int main()
{
	std::vector<ServerConfig> cfgs;
	cfgs.push_back(ServerConfig());

	Server server(cfgs);

	if (!server.startServer())
	{
		std::cerr << "Failed to start server." << std::endl;
		return EXIT_FAILURE;
	}
	server.runServer();
	return EXIT_SUCCESS;
}
