/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringHelpers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 11:12:56 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/26 02:01:48 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <string>

#include "StringHelpers.hpp"

bool StringHelpers::isAllDigits(const std::string &str)
{
	if (str.empty())
	{
		return false;
	}
	for (std::size_t i = 0; i < str.size(); ++i)
	{
		if (!std::isdigit(static_cast<unsigned char>(str[i])))
		{
			return false;
		}
	}
	return true;
}

std::string StringHelpers::toLowercase(const std::string &str)
{
	std::string lowercasedStr = str;
	for (std::size_t i = 0; i < lowercasedStr.size(); ++i)
	{
		lowercasedStr[i] = static_cast<char>(
			std::tolower(static_cast<unsigned char>(lowercasedStr[i])));
	}
	return lowercasedStr;
}
