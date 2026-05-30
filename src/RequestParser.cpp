/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aramos <contact@aramos.dev>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:35:42 by aramos            #+#    #+#             */
/*   Updated: 2026/05/30 12:57:58 by aramos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include <stdexcept>
#include <sstream>

static std::string	trim(const std::string& str)
{
	size_t	start;
	size_t	end;

	start = 0;
	while (start < str.size() && (str[start] == ' ' || str[start] == '\t' || str[start] == '\r'))
		start++;
	end = str.size();
	while (end > start && (str[end - 1] == ' ' || str[end -1] == '\t' || str[end - 1] == '\r'))
		end--;
	return (str.substr(start, end - start));
}

HttpRequest	RequestParser::parse(const std::string& rawRequest)
{
	HttpRequest	req;
	
	std::istringstream	stream(rawRequest);
	std::string			line;

	if (!std::getline(stream, line))
		throw std::runtime_error("Empty request");
	std::istringstream	requestLine(line);
	if(!(requestLine >> req.method >> req.path >> req.version))
		throw(std::runtime_error("Invalid request line"));
	while (std::getline(stream, line))
	{
		if (line == "\r" || line.empty())
			break;
		size_t	separator = line.find(":");
		if (separator == std::string::npos)
			continue;
		std::string	key = trim(line.substr(0, separator));
		std::string	value = trim(line.substr(separator + 1));
		req.headers[key] = value;
	}
	std::string	body;
	while (std::getline(stream, line))
	{
		body += line;
		if (!stream.eof())
			body += "\n";
	}
	req.body = body;
	return req;
}
