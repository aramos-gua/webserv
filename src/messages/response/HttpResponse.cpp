/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 22:26:14 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpFieldHelpers.hpp"
#include "HttpResponse.hpp"
#include "HttpSyntax.hpp"

HttpResponse::HttpResponse(void)
	: version(HTTP_1_1), statusCode(OK), bodyIsExternal(false),
	  externalBodyLength(0)
{
}

HttpResponse::HttpResponse(const HttpResponse &other)
	: version(other.version), statusCode(other.statusCode),
	  headers(other.headers), body(other.body),
	  bodyIsExternal(other.bodyIsExternal),
	  externalBodyLength(other.externalBodyLength)
{
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other)
{
	if (this != &other)
	{
		version = other.version;
		statusCode = other.statusCode;
		headers = other.headers;
		body = other.body;
		bodyIsExternal = other.bodyIsExternal;
		externalBodyLength = other.externalBodyLength;
	}
	return *this;
}

HttpResponse::~HttpResponse(void)
{
}

HttpVersion HttpResponse::getVersion(void) const
{
	return version;
}

HttpStatusCode HttpResponse::getStatusCode(void) const
{
	return statusCode;
}

const t_http_headers &HttpResponse::getHeaders(void) const
{
	return headers;
}

const std::string &HttpResponse::getBody(void) const
{
	return body;
}

std::size_t HttpResponse::getBodyLength(void) const
{
	return bodyIsExternal ? externalBodyLength : body.size();
}

bool HttpResponse::getWhetherBodyIsExternal(void) const
{
	return bodyIsExternal;
}

void HttpResponse::setVersion(HttpVersion version)
{
	this->version = version;
}

void HttpResponse::setStatusCode(HttpStatusCode statusCode)
{
	this->statusCode = statusCode;
}

bool HttpResponse::setHeader(const std::string &name, const std::string &value)
{
	// What the field costs on the wire: "name: value" and its terminator.
	static const std::size_t SEPARATOR_AND_TERMINATOR_LENGTH =
		HttpSyntax::SEPARATOR.length() + HttpSyntax::CRLF.length();

	if (!HttpFieldHelpers::getWhetherFieldNameIsValid(name))
	{
		return false;
	}
	if (name.size() + value.size() + SEPARATOR_AND_TERMINATOR_LENGTH >
	    MAX_HEADER_LINE_LENGTH)
	{
		return false;
	}
	if (headers.size() >= MAX_HEADER_COUNT &&
	    headers.find(name) == headers.end())
	{
		return false;
	}
	headers[name] = value;
	return true;
}

void HttpResponse::setBody(const std::string &body)
{
	this->body = body;
	bodyIsExternal = false;
	externalBodyLength = 0;
}

void HttpResponse::setExternalBody(std::size_t length)
{
	body.clear();
	bodyIsExternal = true;
	externalBodyLength = length;
}
