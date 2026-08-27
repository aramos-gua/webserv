/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:35:42 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 14:06:44 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstdlib>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>

#include "HttpRequestParser.hpp"
#include "HttpStatusCode.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::string SINGLE_OCCURRENCE_FIELD_NAME_ARRAY[] = {
	"authorization", "content-length", "content-type", "cookie",  "date",
	"from",          "host",           "origin",       "referer", "user-agent",
};

// Field names that may appear only once. Repeating any of these is either a
// framing question with no safe answer (host, content-length) or produces a
// value that comma-joining would corrupt rather than combine — user-agent
// separates its product tokens with spaces, and cookie with semicolons.
static const std::set<std::string> SINGLE_OCCURRENCE_FIELD_NAMES(
	SINGLE_OCCURRENCE_FIELD_NAME_ARRAY,
	SINGLE_OCCURRENCE_FIELD_NAME_ARRAY +
		(sizeof(SINGLE_OCCURRENCE_FIELD_NAME_ARRAY) / sizeof(std::string)));

// NOLINTEND(bugprone-throwing-static-initialization)

/* ************************************************************************** */
/*                              CONSTRUCTOR                                   */
/* ************************************************************************** */
HttpRequestParser::HttpRequestParser(std::size_t maxBodySize)
	: state(PARSE_REQUEST_LINE), errorStatusCode(NONE),
	  maxBodySize(maxBodySize), bodyBytesNeeded(0), headerCount(0)
{
}

/* ************************************************************************** */
/*                              COPY CONSTRUCTOR                              */
/* ************************************************************************** */
HttpRequestParser::HttpRequestParser(const HttpRequestParser &other)
	: state(other.state), buffer(other.buffer), request(other.request),
	  errorStatusCode(other.errorStatusCode), maxBodySize(other.maxBodySize),
	  bodyBytesNeeded(other.bodyBytesNeeded), headerCount(other.headerCount)
{
}

/* ************************************************************************** */
/*                          COPY ASSIGNMENT OPERATOR                          */
/* ************************************************************************** */
HttpRequestParser &HttpRequestParser::operator=(const HttpRequestParser &other)
{
	if (this != &other)
	{
		state = other.state;
		buffer = other.buffer;
		request = other.request;
		errorStatusCode = other.errorStatusCode;
		maxBodySize = other.maxBodySize;
		bodyBytesNeeded = other.bodyBytesNeeded;
		headerCount = other.headerCount;
	}
	return *this;
}

/* ************************************************************************** */
/*                               DESTRUCTOR                                   */
/* ************************************************************************** */
HttpRequestParser::~HttpRequestParser(void)
{
}

/* ************************************************************************** */
/*                               MEMBER FUNCTIONS                             */
/* ************************************************************************** */
std::size_t HttpRequestParser::getMinimumConfirmedLineLength(
	const std::string &str, std::size_t terminatorPos)
{
	if (terminatorPos != std::string::npos)
	{
		return terminatorPos;
	}
	// Without a terminator the line is at least the whole buffer, except that
	// a trailing CR may be the first half of the CRLF that ends it, and so
	// isn't yet known to be content. Without that allowance a line of exactly
	// the maximum length would be rejected whenever its CR and LF happened to
	// arrive in separate reads.
	if (!str.empty() && str[str.size() - 1] == '\r')
	{
		return str.size() - 1;
	}
	return str.size();
}

bool HttpRequestParser::isValidFieldName(const std::string &fieldName)
{
	static const std::string TOKEN_SYMBOLS = "!#$%&'*+-.^_`|~";

	if (fieldName.empty())
	{
		return false;
	}
	for (std::size_t i = 0; i < fieldName.size(); ++i)
	{
		unsigned char character = static_cast<unsigned char>(fieldName[i]);
		if (!isalnum(character) && TOKEN_SYMBOLS.find(static_cast<char>(
									   character)) == std::string::npos)
		{
			return false;
		}
	}
	return true;
}

std::string HttpRequestParser::trim(const std::string &str)
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

bool HttpRequestParser::isValidContentLength(const std::string &str,
                                             std::size_t &out)
{
	static const std::size_t DECIMAL_BASE = 10;

	if (str.empty())
	{
		return false;
	}
	std::size_t value = 0;
	for (std::size_t i = 0; i < str.size(); ++i)
	{
		if (!isdigit(static_cast<unsigned char>(str[i])))
		{
			return false;
		}
		std::size_t digit = str[i] - '0';
		if (value >
		    (std::numeric_limits<std::size_t>::max() - digit) / DECIMAL_BASE)
		{
			return false;
		}
		value = (value * DECIMAL_BASE) + digit;
	}
	out = value;
	return true;
}

void HttpRequestParser::reset(void)
{
	state = PARSE_REQUEST_LINE;
	buffer.clear();
	request = HttpRequest();
	errorStatusCode = NONE;
	bodyBytesNeeded = 0;
	headerCount = 0;
}

const HttpRequest &HttpRequestParser::getRequest(void) const
{
	return request;
}

HttpStatusCode HttpRequestParser::getErrorStatusCode(void) const
{
	return errorStatusCode;
}

bool HttpRequestParser::parseRequestLine(void)
{
	static const std::size_t MAX_REQUEST_LINE = 8192;
	std::size_t pos = buffer.find("\r\n");
	// The line is exactly pos bytes once its terminator has arrived, and at
	// least buffer.size() bytes while it hasn't. An oversized line can never
	// become valid either way, so the limit is checked before the terminator
	// is required — otherwise a line that arrives complete in a single read
	// would never be measured at all.
	std::size_t minimumConfirmedLineLength =
		getMinimumConfirmedLineLength(buffer, pos);
	if (minimumConfirmedLineLength > MAX_REQUEST_LINE)
	{
		state = PARSE_ERROR;
		errorStatusCode = URI_TOO_LONG;
		return false;
	}
	if (pos == std::string::npos)
	{
		return false;
	}
	std::string line = buffer.substr(0, pos);
	buffer.erase(0, pos + 2);
	std::istringstream iss(line);
	if (!(iss >> request.method >> request.path >> request.version))
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	state = PARSE_HEADERS;
	return true;
}

bool HttpRequestParser::parseHeaders(void)
{
	static const std::size_t MAX_HEADERS_LINE = 8192;
	std::size_t pos = buffer.find("\r\n");
	// Measured the same way as the request line above: whether or not the
	// terminator has arrived, a line already over the limit is rejected.
	std::size_t minimumConfirmedLineLength =
		getMinimumConfirmedLineLength(buffer, pos);
	if (minimumConfirmedLineLength > MAX_HEADERS_LINE)
	{
		state = PARSE_ERROR;
		errorStatusCode = REQUEST_HEADER_FIELDS_TOO_LARGE;
		return false;
	}
	if (pos == std::string::npos)
	{
		return false;
	}
	std::string line = buffer.substr(0, pos);
	buffer.erase(0, pos + 2);
	if (line.empty())
	{
		// RFC 9112 requires a server to answer 400 to an HTTP/1.1 request that
		// lacks a Host, and to one whose Host value is invalid — an empty
		// value included, since it names no authority. HTTP/1.0 predates
		// virtual hosting and carries neither requirement, so the check is
		// deliberately version-specific.
		std::map<std::string, std::string>::const_iterator hostField =
			request.headers.find("host");
		if (request.version == "HTTP/1.1" &&
		    (hostField == request.headers.end() || hostField->second.empty()))
		{
			state = PARSE_ERROR;
			errorStatusCode = BAD_REQUEST;
			return false;
		}
		std::map<std::string, std::string>::const_iterator contentLengthField =
			request.headers.find("content-length");
		if (contentLengthField != request.headers.end())
		{
			if (!isValidContentLength(contentLengthField->second,
			                          bodyBytesNeeded))
			{
				state = PARSE_ERROR;
				errorStatusCode = BAD_REQUEST;
				return false;
			}
			if (bodyBytesNeeded > maxBodySize)
			{
				state = PARSE_ERROR;
				errorStatusCode = CONTENT_TOO_LARGE;
				return false;
			}
		}
		state = PARSE_BODY;
		return true;
	}
	std::size_t sep = line.find(":");
	if (sep == std::string::npos)
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	static const std::size_t MAX_HEADER_COUNT = 100;
	if (++headerCount > MAX_HEADER_COUNT)
	{
		state = PARSE_ERROR;
		errorStatusCode = REQUEST_HEADER_FIELDS_TOO_LARGE;
		return false;
	}
	// RFC 9110 defines a field name as one or more "tchar"s, and whitespace is
	// not one of them. Trimming "Host : x" into "Host" rather than rejecting it
	// is how a recipient ends up disagreeing with the next hop about where a
	// header begins, which is the basis of request smuggling — so the name is
	// validated exactly as it arrived.
	std::string key = line.substr(0, sep);
	if (!isValidFieldName(key))
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	for (std::size_t i = 0; i < key.size(); ++i)
	{
		key[i] = static_cast<char>(tolower(static_cast<unsigned char>(key[i])));
	}
	std::string val = trim(line.substr(sep + 1));
	std::map<std::string, std::string>::iterator existingField =
		request.headers.find(key);
	if (existingField == request.headers.end())
	{
		request.headers[key] = val;
		return true;
	}
	if (SINGLE_OCCURRENCE_FIELD_NAMES.find(key) !=
	    SINGLE_OCCURRENCE_FIELD_NAMES.end())
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	// RFC 9110: repeating a list-valued field means the same as sending one
	// field line whose values are joined, in order, by commas.
	existingField->second += ", " + val;
	return true;
}

bool HttpRequestParser::parseBody(void)
{
	if (bodyBytesNeeded == 0)
	{
		state = PARSE_DONE;
		return true;
	}
	if (buffer.size() < bodyBytesNeeded)
	{
		return false;
	}
	request.body = buffer.substr(0, bodyBytesNeeded);
	request.contentLength = bodyBytesNeeded;
	buffer.erase(0, bodyBytesNeeded);
	state = PARSE_DONE;
	return true;
}

HttpRequestParser::Result HttpRequestParser::feed(const char *data,
                                                  std::size_t len)
{
	if (state == PARSE_ERROR)
	{
		return ERROR;
	}
	if (state == PARSE_DONE)
	{
		return COMPLETE;
	}
	buffer.append(data, len);
	bool progress = true;
	while (progress)
	{
		progress = false;
		if (state == PARSE_REQUEST_LINE)
		{
			progress = parseRequestLine();
		}
		else if (state == PARSE_HEADERS)
		{
			progress = parseHeaders();
		}
		else if (state == PARSE_BODY)
		{
			progress = parseBody();
		}
	}
	if (state == PARSE_DONE)
	{
		return COMPLETE;
	}
	if (state == PARSE_ERROR)
	{
		return ERROR;
	}
	return INCOMPLETE;
}
