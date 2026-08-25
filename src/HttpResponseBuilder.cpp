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

#include <sstream>

#include "HttpResponseBuilder.hpp"

std::string HttpResponseBuilder::sanitizeHeaderValue(const std::string &s)
{
	std::string out;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] != '\r' && s[i] != '\n')
		{
			out += s[i];
		}
	}
	return out;
}

std::string HttpResponseBuilder::build(const HttpResponse &res)
{
	std::ostringstream out;
	out << res.version << " " << res.statusCode << " "
		<< sanitizeHeaderValue(res.description) << "\r\n";
	std::map<std::string, std::string>::const_iterator it;
	for (it = res.headers.begin(); it != res.headers.end(); ++it)
	{
		if (it->first == "Content-Length" || it->first == "content-length")
		{
			continue;
		}
		out << it->first << ": " << sanitizeHeaderValue(it->second) << "\r\n";
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
