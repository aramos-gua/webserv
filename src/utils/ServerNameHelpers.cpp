/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNameHelpers.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 07:34:35 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/02 07:34:39 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>

#include "ServerNameHelpers.hpp"

bool ServerNameHelpers::isValidServerName(const std::string &serverName)
{
	if (serverName.empty() || serverName[0] == '-' || serverName[0] == '.')
	{
		return false;
	}
	if (serverName[serverName.size() - 1] == '.')
	{
		return false;
	}
	unsigned char previousChar = '\0';
	for (std::size_t i = 0; i < serverName.size(); ++i)
	{
		unsigned char currentChar = static_cast<unsigned char>(serverName[i]);
		if (!std::isalnum(currentChar) && currentChar != '.' &&
		    currentChar != '-')
		{
			return false;
		}
		if (currentChar == '.' && previousChar == '.')
		{
			return false;
		}
		previousChar = currentChar;
	}
	return true;
}
