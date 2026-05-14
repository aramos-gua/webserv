/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 03:06:27 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 23:56:08 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <exception>
#include <iostream>

#include "AConfigToken.hpp"
#include "ConfigLexer.hpp"

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
		ConfigLexer configLexer(argv[1]);
		AConfigToken *configToken = NULL;
		while (true)
		{
			configLexer >> &configToken;
			AConfigToken::Type configTokenType = configToken->getType();
			std::cout << "- " << *configToken << std::endl;
			delete configToken;
			if (configTokenType == AConfigToken::TERMINATING)
			{
				break;
			}
		}
		configLexer.throwForUnclosedQuoteOrUnterminatedEscape();
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
