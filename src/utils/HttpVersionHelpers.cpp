/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersionHelpers.cpp                                   :+:      :+: :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:36:38 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 16:27:17 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <set>
#include <stdexcept>
#include <utility>

#include "HttpVersionHelpers.hpp"
#include "StringBase.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::pair<std::string, HttpVersion> HTTP_VERSION_STRING_PAIRS[] = {
	std::pair<std::string, HttpVersion>("HTTP/1.0", HTTP_1_0),
	std::pair<std::string, HttpVersion>("HTTP/1.1", HTTP_1_1),
	std::pair<std::string, HttpVersion>("HTTP/2", HTTP_2),
	std::pair<std::string, HttpVersion>("HTTP/2.0", HTTP_2),
	std::pair<std::string, HttpVersion>("HTTP/3", HTTP_3),
	std::pair<std::string, HttpVersion>("HTTP/3.0", HTTP_3),
};

static const std::pair<HttpVersion, std::string> HTTP_STRING_VERSION_PAIRS[] = {
	std::pair<HttpVersion, std::string>(HTTP_1_0, "HTTP/1.0"),
	std::pair<HttpVersion, std::string>(HTTP_1_1, "HTTP/1.1"),
	std::pair<HttpVersion, std::string>(HTTP_2, "HTTP/2"),
	std::pair<HttpVersion, std::string>(HTTP_3, "HTTP/3"),
};

const std::map<std::string, HttpVersion> HttpVersionHelpers::
	HTTP_VERSIONS_FOR_STRINGS(
		HTTP_VERSION_STRING_PAIRS,
		HTTP_VERSION_STRING_PAIRS +
			(sizeof(HTTP_VERSION_STRING_PAIRS) /
             sizeof(std::pair<std::string, HttpVersion>)));

const std::map<HttpVersion, std::string> HttpVersionHelpers::
	STRINGS_FOR_HTTP_VERSIONS(
		HTTP_STRING_VERSION_PAIRS,
		HTTP_STRING_VERSION_PAIRS +
			(sizeof(HTTP_STRING_VERSION_PAIRS) /
             sizeof(std::pair<HttpVersion, std::string>)));

static const HttpVersion SUPPORTED_HTTP_VERSION_ARRAY[] = {
	HTTP_1_0,
	HTTP_1_1,
};

const std::set<HttpVersion> HttpVersionHelpers::SUPPORTED_HTTP_VERSIONS(
	SUPPORTED_HTTP_VERSION_ARRAY,
	SUPPORTED_HTTP_VERSION_ARRAY +
		(sizeof(SUPPORTED_HTTP_VERSION_ARRAY) / sizeof(HttpVersion)));

// NOLINTEND(bugprone-throwing-static-initialization)

HttpVersion HttpVersionHelpers::getHttpVersionForString(
	const std::string &versionString)
{
	std::map<std::string, HttpVersion>::const_iterator iterator =
		HTTP_VERSIONS_FOR_STRINGS.find(versionString);
	if (iterator == HTTP_VERSIONS_FOR_STRINGS.end())
	{
		throw std::out_of_range(StringBase() << "Unknown HTTP version \""
		                                     << versionString << "\"");
	}
	return iterator->second;
}

std::string HttpVersionHelpers::getStringForHttpVersion(HttpVersion version)
{
	std::map<HttpVersion, std::string>::const_iterator iterator =
		STRINGS_FOR_HTTP_VERSIONS.find(version);
	if (iterator == STRINGS_FOR_HTTP_VERSIONS.end())
	{
		throw std::out_of_range("Unknown HTTP version");
	}
	return iterator->second;
}

bool HttpVersionHelpers::getWhetherVersionIsSupported(HttpVersion version)
{
	return SUPPORTED_HTTP_VERSIONS.find(version) !=
	       SUPPORTED_HTTP_VERSIONS.end();
}
