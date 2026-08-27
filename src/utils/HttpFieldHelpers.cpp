/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpFieldHelpers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 21:40:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 21:40:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstddef>

#include "HttpFieldHelpers.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

const std::string HttpFieldHelpers::TOKEN_SYMBOLS = "!#$%&'*+-.^_`|~";

// NOLINTEND(bugprone-throwing-static-initialization)

bool HttpFieldHelpers::getWhetherFieldNameIsValid(const std::string &fieldName)
{
	if (fieldName.empty())
	{
		return false;
	}
	for (std::size_t i = 0; i < fieldName.size(); ++i)
	{
		unsigned char character = static_cast<unsigned char>(fieldName[i]);
		if (!isalnum(character) && TOKEN_SYMBOLS.find(static_cast<char>(
									   character)) == std::string::npos)
		{
			return false;
		}
	}
	return true;
}
