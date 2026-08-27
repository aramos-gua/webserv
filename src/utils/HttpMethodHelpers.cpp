/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethodHelpers.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:45:30 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/01 05:04:04 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <utility>

#include "HttpMethodHelpers.hpp"
#include "StringBase.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::pair<std::string, HttpMethod> HTTP_METHOD_STRING_PAIRS[] = {
	std::pair<std::string, HttpMethod>("GET", GET),
	std::pair<std::string, HttpMethod>("HEAD", HEAD),
	std::pair<std::string, HttpMethod>("POST", POST),
	std::pair<std::string, HttpMethod>("PUT", PUT),
	std::pair<std::string, HttpMethod>("PATCH", PATCH),
	std::pair<std::string, HttpMethod>("DELETE", DELETE),
};

static const std::pair<HttpMethod, std::string> HTTP_STRING_METHOD_PAIRS[] = {
	std::pair<HttpMethod, std::string>(GET, "GET"),
	std::pair<HttpMethod, std::string>(HEAD, "HEAD"),
	std::pair<HttpMethod, std::string>(POST, "POST"),
	std::pair<HttpMethod, std::string>(PUT, "PUT"),
	std::pair<HttpMethod, std::string>(PATCH, "PATCH"),
	std::pair<HttpMethod, std::string>(DELETE, "DELETE"),
};

const std::map<std::string, HttpMethod> HttpMethodHelpers::
	HTTP_METHODS_FOR_STRINGS(HTTP_METHOD_STRING_PAIRS,
                             HTTP_METHOD_STRING_PAIRS +
                                 (sizeof(HTTP_METHOD_STRING_PAIRS) /
                                  sizeof(std::pair<std::string, HttpMethod>)));

const std::map<HttpMethod, std::string> HttpMethodHelpers::
	STRINGS_FOR_HTTP_METHODS(HTTP_STRING_METHOD_PAIRS,
                             HTTP_STRING_METHOD_PAIRS +
                                 (sizeof(HTTP_STRING_METHOD_PAIRS) /
                                  sizeof(std::pair<HttpMethod, std::string>)));

// NOLINTEND(bugprone-throwing-static-initialization)

HttpMethod HttpMethodHelpers::getHttpMethodForString(
	const std::string &methodString)
{
	std::map<std::string, HttpMethod>::const_iterator iterator =
		HTTP_METHODS_FOR_STRINGS.find(methodString);
	if (iterator == HTTP_METHODS_FOR_STRINGS.end())
	{
		throw std::out_of_range(StringBase() << "Unknown HTTP method \""
		                                     << methodString << "\"");
	}
	return iterator->second;
}

std::string HttpMethodHelpers::getStringForHttpMethod(HttpMethod method)
{
	std::map<HttpMethod, std::string>::const_iterator iterator =
		STRINGS_FOR_HTTP_METHODS.find(method);
	if (iterator == STRINGS_FOR_HTTP_METHODS.end())
	{
		throw std::out_of_range(StringBase() << "Unknown HTTP method "
		                                     << static_cast<int>(method));
	}
	return iterator->second;
}
