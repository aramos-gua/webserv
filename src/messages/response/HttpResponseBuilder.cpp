/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 21:43:18 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 21:49:29 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <stdexcept>

#include "HttpDateHelpers.hpp"
#include "HttpResponseBuilder.hpp"
#include "HttpStatusCodeHelpers.hpp"
#include "HttpSyntax.hpp"
#include "HttpVersionHelpers.hpp"
#include "StringHelpers.hpp"

static const char *const SERVER_NAME = "penguinx";

// The fields the builder decides for itself. Named once and used both to look
// for a caller's version and to write our own, so the two cannot disagree.
static const char *const CONNECTION_HEADER = "Connection";
static const char *const CONTENT_LENGTH_HEADER = "Content-Length";
static const char *const DATE_HEADER = "Date";
static const char *const SERVER_HEADER = "Server";

static std::string getReasonPhrase(HttpStatusCode statusCode)
{
	try
	{
		return HttpStatusCodeHelpers::getReasonPhraseForStatusCode(statusCode);
	}
	catch (const std::out_of_range &)
	{
		return "";
	}
}

// Guarded for the same reason, but falling back to HTTP/1.1 rather than an
// empty string, because a status line without a version is malformed whereas
// naming the highest version this server speaks is at worst inaccurate.
static std::string getVersionString(HttpVersion version)
{
	try
	{
		return HttpVersionHelpers::getStringForHttpVersion(version);
	}
	catch (const std::out_of_range &)
	{
		return HttpVersionHelpers::getStringForHttpVersion(HTTP_1_1);
	}
}

static bool hasHeader(const HttpResponse &response, const std::string &name)
{
	return response.getHeaders().find(name) != response.getHeaders().end();
}

static void appendStatusLine(std::ostringstream &out,
                             const HttpResponse &response)
{
	out << getVersionString(response.getVersion()) << " "
		<< static_cast<int>(response.getStatusCode()) << " "
		<< getReasonPhrase(response.getStatusCode()) << HttpSyntax::CRLF;
}

static void appendCallerHeaders(std::ostringstream &out,
                                const HttpResponse &response)
{
	t_http_headers::const_iterator contentLength =
		response.getHeaders().find(CONTENT_LENGTH_HEADER);
	t_http_headers::const_iterator headerIterator;

	for (headerIterator = response.getHeaders().begin();
	     headerIterator != response.getHeaders().end(); ++headerIterator)
	{
		if (headerIterator == contentLength)
		{
			continue;
		}
		out << headerIterator->first << ": "
			<< StringHelpers::removeLineBreaks(headerIterator->second)
			<< HttpSyntax::CRLF;
	}
}

// Each supplied only where the caller has not spoken for itself.
static void appendDefaultHeaders(std::ostringstream &out,
                                 const HttpResponse &response)
{
	if (!hasHeader(response, CONNECTION_HEADER))
	{
		out << CONNECTION_HEADER << ": close" << HttpSyntax::CRLF;
	}
	if (!hasHeader(response, DATE_HEADER))
	{
		// RFC 9110 requires an origin server to send Date whenever it can tell
		// the time, so this is omitted only when the clock cannot be read.
		std::string httpDate = HttpDateHelpers::getCurrentHttpDate();
		if (!httpDate.empty())
		{
			out << DATE_HEADER << ": " << httpDate << HttpSyntax::CRLF;
		}
	}
	if (!hasHeader(response, SERVER_HEADER))
	{
		out << SERVER_HEADER << ": " << SERVER_NAME << HttpSyntax::CRLF;
	}
}

// 204 and 304 cannot carry a body, so they get no Content-Length either: RFC
// 9110 forbids the field outright on a 204, and on a 304 a length of zero would
// claim the resource is empty rather than unchanged.
//
// The body is dropped along with it, even if a caller supplied one. A body with
// no Content-Length has nothing to say where it ends, so a recipient reading a
// persistent connection would take it for the start of the next response — the
// framing hole this field exists to close.
static void appendBody(std::ostringstream &out, const HttpResponse &response)
{
	if (HttpStatusCodeHelpers::takesNoValue(response.getStatusCode()))
	{
		out << HttpSyntax::CRLF;
		return;
	}
	out << CONTENT_LENGTH_HEADER << ": " << response.getBodyLength()
		<< HttpSyntax::CRLF;
	out << HttpSyntax::CRLF;
	// An external body is not here to be written. build() produces the head and
	// the caller streams the rest from whatever holds it.
	if (!response.getWhetherBodyIsExternal())
	{
		out << response.getBody();
	}
}

std::string HttpResponseBuilder::build(const HttpResponse &res)
{
	std::ostringstream out;

	appendStatusLine(out, res);
	appendCallerHeaders(out, res);
	appendDefaultHeaders(out, res);
	appendBody(out, res);
	return out.str();
}
