/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 18:00:17 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utility>

#include "HttpRequest.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::string SINGLE_OCCURRENCE_HEADER_NAME_ARRAY[] = {
	"authorization", "content-length", "content-type", "cookie",  "date",
	"from",          "host",           "origin",       "referer", "user-agent",
};

const std::set<std::string> HttpRequest::SINGLE_OCCURRENCE_HEADER_NAMES(
	SINGLE_OCCURRENCE_HEADER_NAME_ARRAY,
	SINGLE_OCCURRENCE_HEADER_NAME_ARRAY +
		(sizeof(SINGLE_OCCURRENCE_HEADER_NAME_ARRAY) / sizeof(std::string)));

// NOLINTEND(bugprone-throwing-static-initialization)

HttpRequest::HttpRequest(void): method(NO_METHOD), version(NO_VERSION)
{
}

HttpRequest::HttpRequest(const HttpRequest &other)
	: method(other.method), path(other.path), version(other.version),
	  headers(other.headers), body(other.body)
{
}

HttpRequest &HttpRequest::operator=(const HttpRequest &other)
{
	if (this != &other)
	{
		method = other.method;
		path = other.path;
		version = other.version;
		headers = other.headers;
		body = other.body;
	}
	return *this;
}

HttpRequest::~HttpRequest(void)
{
}

HttpMethod HttpRequest::getMethod(void) const
{
	return method;
}

const std::string &HttpRequest::getPath(void) const
{
	return path;
}

HttpVersion HttpRequest::getVersion(void) const
{
	return version;
}

const HttpRequest::t_headers &HttpRequest::getHeaders(void) const
{
	return headers;
}

const std::string &HttpRequest::getBody(void) const
{
	return body;
}

void HttpRequest::setMethod(HttpMethod method)
{
	this->method = method;
}

void HttpRequest::setPath(const std::string &path)
{
	this->path = path;
}

void HttpRequest::setVersion(HttpVersion version)
{
	this->version = version;
}

bool HttpRequest::addHeader(const std::string &name, const std::string &value)
{
	t_headers::iterator existingHeader = headers.find(name);

	if (existingHeader == headers.end())
	{
		headers[name] = value;
		return true;
	}
	if (SINGLE_OCCURRENCE_HEADER_NAMES.find(name) !=
	    SINGLE_OCCURRENCE_HEADER_NAMES.end())
	{
		return false;
	}
	existingHeader->second += ", " + value;
	return true;
}

void HttpRequest::appendToBody(const std::string &data)
{
	body += data;
}
