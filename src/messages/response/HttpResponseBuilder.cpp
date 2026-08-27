/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aramos <contact@aramos.dev>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 21:43:18 by aramos            #+#    #+#             */
/*   Updated: 2026/08/23 21:43:21 by aramos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <sstream>

#include "HttpResponseBuilder.hpp"

std::string HttpResponseBuilder::sanitizeHeaderValue(const std::string &str)
{
	std::string out;
	for (std::size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] != '\r' && str[i] != '\n')
		{
			out += str[i];
		}
	}
	return out;
}

std::string HttpResponseBuilder::build(const HttpResponse &res)
{
	std::ostringstream out;
	out << res.version << " " << res.statusCode << " "
		<< sanitizeHeaderValue(res.description) << "\r\n";
	std::map<std::string, std::string>::const_iterator headerIterator;
	for (headerIterator = res.headers.begin();
	     headerIterator != res.headers.end(); ++headerIterator)
	{
		if (headerIterator->first == "Content-Length" ||
		    headerIterator->first == "content-length")
		{
			continue;
		}
		out << headerIterator->first << ": "
			<< sanitizeHeaderValue(headerIterator->second) << "\r\n";
	}
	bool hasConnection =
		res.headers.count("Connection") || res.headers.count("connection");
	if (!hasConnection)
	{
		out << "Connection: close\r\n";
	}
	out << "Content-Length: " << res.body.size() << "\r\n";
	out << "\r\n";
	out << res.body;
	return out.str();
}
