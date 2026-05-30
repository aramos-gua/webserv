/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 19:18:39 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/03 13:54:25 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <exception>
#include <iostream>

#include "ConfigParser.hpp"
#include "MainConfig.hpp"

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
		// TODO: add more steps
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
