/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringHelpers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 11:12:56 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 19:13:29 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <string>
#include <vector>

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

std::string StringHelpers::trim(const std::string &str)
{
	std::size_t start;
	std::size_t end;

	start = 0;
	while (start < str.size() &&
	       (str[start] == ' ' || str[start] == '\t' || str[start] == '\r'))
	{
		start++;
	}
	end = str.size();
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t' ||
	                       str[end - 1] == '\r'))
	{
		end--;
	}
	return str.substr(start, end - start);
}

std::vector<std::string> StringHelpers::splitOnCommas(const std::string &str)
{
	std::vector<std::string> elements;
	std::size_t start = 0;

	while (true)
	{
		std::size_t separator = str.find(',', start);
		if (separator == std::string::npos)
		{
			elements.push_back(trim(str.substr(start)));
			return elements;
		}
		elements.push_back(trim(str.substr(start, separator - start)));
		start = separator + 1;
	}
}

std::string StringHelpers::removeLineBreaks(const std::string &str)
{
	std::string strWithoutLineBreaks;

	for (std::size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] != '\r' && str[i] != '\n')
		{
			strWithoutLineBreaks += str[i];
		}
	}
	return strWithoutLineBreaks;
}
