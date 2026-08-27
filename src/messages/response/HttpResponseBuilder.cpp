/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 21:43:18 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 20:20:58 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <ctime>
#include <iomanip>
#include <set>
#include <sstream>
#include <stdexcept>

#include "HttpResponseBuilder.hpp"
#include "HttpStatusCodeHelpers.hpp"
#include "HttpSyntax.hpp"
#include "HttpVersionHelpers.hpp"
#include "StringHelpers.hpp"

static const char *const SERVER_NAME = "penguinx";

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

// NOTE: fixed English abbreviations according to RFC 9110, not locale-dependent
static const char *const DAY_NAMES[] = {"Sun", "Mon", "Tue", "Wed",
                                        "Thu", "Fri", "Sat"};
static const char *const MONTH_NAMES[] = {"Jan", "Feb", "Mar", "Apr",
                                          "May", "Jun", "Jul", "Aug",
                                          "Sep", "Oct", "Nov", "Dec"};

// IMF-fixdate, which RFC 9110 requires a sender to use and which is always
// GMT: "Sun, 06 Nov 1994 08:49:37 GMT". Returns empty if the clock cannot be
// read, and the caller then omits the field rather than sending a wrong one.
static std::string getCurrentHttpDate(void)
{
	// std::tm counts years from 1900.
	static const int TM_YEAR_EPOCH = 1900;

	std::time_t now = std::time(NULL);
	std::tm *utc = std::gmtime(&now);

	if (utc == NULL)
	{
		return "";
	}
	std::ostringstream out;
	out << DAY_NAMES[utc->tm_wday] << ", " << std::setfill('0') << std::setw(2)
		<< utc->tm_mday << " " << MONTH_NAMES[utc->tm_mon] << " "
		<< (utc->tm_year + TM_YEAR_EPOCH) << " " << std::setw(2) << utc->tm_hour
		<< ":" << std::setw(2) << utc->tm_min << ":" << std::setw(2)
		<< utc->tm_sec << " GMT";
	return out.str();
}

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

std::string HttpResponseBuilder::build(const HttpResponse &res)
{
	std::ostringstream out;
	out << getVersionString(res.getVersion()) << " "
		<< static_cast<int>(res.getStatusCode()) << " "
		<< getReasonPhrase(res.getStatusCode()) << HttpSyntax::CRLF;
	// Field names are case-insensitive, so the two the builder decides for
	// itself have to be recognised however the caller spelled them. Matching
	// exact spellings let "Content-length" through, and the response then went
	// out carrying two Content-Length headers — which is how a recipient and
	// the next hop come to disagree about where the body ends.
	t_http_headers::const_iterator headerIterator;
	std::set<std::string> callerSuppliedNames;
	for (headerIterator = res.getHeaders().begin();
	     headerIterator != res.getHeaders().end(); ++headerIterator)
	{
		std::string lowercaseName =
			StringHelpers::toLowercase(headerIterator->first);
		if (lowercaseName == "content-length")
		{
			continue;
		}
		callerSuppliedNames.insert(lowercaseName);
		out << headerIterator->first << ": "
			<< StringHelpers::removeLineBreaks(headerIterator->second)
			<< HttpSyntax::CRLF;
	}
	// Defaults, each supplied only when the caller has not spoken for itself.
	if (callerSuppliedNames.find("connection") == callerSuppliedNames.end())
	{
		out << "Connection: close\r\n";
	}
	if (callerSuppliedNames.find("date") == callerSuppliedNames.end())
	{
		// RFC 9110 requires an origin server to send Date whenever it can tell
		// the time, so this is omitted only when the clock cannot be read.
		std::string httpDate = getCurrentHttpDate();
		if (!httpDate.empty())
		{
			out << "Date: " << httpDate << HttpSyntax::CRLF;
		}
	}
	if (callerSuppliedNames.find("server") == callerSuppliedNames.end())
	{
		out << "Server: " << SERVER_NAME << HttpSyntax::CRLF;
	}
	// 204 and 304 cannot carry a body, so they get no Content-Length either:
	// RFC 9110 forbids the field outright on a 204, and on a 304 a length of
	// zero would claim the resource is empty rather than unchanged.
	//
	// The body is dropped along with it, even if a caller supplied one. A body
	// with no Content-Length has nothing to say where it ends, so a recipient
	// reading a persistent connection would take it for the start of the next
	// response — the framing hole this field exists to close.
	if (!HttpStatusCodeHelpers::takesNoValue(res.getStatusCode()))
	{
		out << "Content-Length: " << res.getBodyLength() << HttpSyntax::CRLF;
		out << HttpSyntax::CRLF;
		// An external body is not here to be written. build() produces the head
		// and the caller streams the rest from whatever holds it.
		if (!res.getWhetherBodyIsExternal())
		{
			out << res.getBody();
		}
		return out.str();
	}
	out << HttpSyntax::CRLF;
	return out.str();
}
