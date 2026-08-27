/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:35:42 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 18:26:18 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <stdexcept>

#include "HttpMethodHelpers.hpp"
#include "HttpRequestParser.hpp"
#include "HttpStatusCode.hpp"
#include "HttpVersionHelpers.hpp"
#include "StringHelpers.hpp"
#include "TransferEncodingHelpers.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

const std::string HttpRequestParser::TERMINATOR = "\r\n";

// NOLINTEND(bugprone-throwing-static-initialization)

HttpRequestParser::HttpRequestParser(std::size_t maxBodySize)
	: state(PARSE_REQUEST_LINE), errorStatusCode(NO_STATUS_CODE),
	  maxBodySize(maxBodySize), bodyBytesNeeded(0), headerCount(0),
	  chunkBytesNeeded(0)
{
}

HttpRequestParser::HttpRequestParser(const HttpRequestParser &other)
	: state(other.state), buffer(other.buffer), request(other.request),
	  errorStatusCode(other.errorStatusCode), maxBodySize(other.maxBodySize),
	  bodyBytesNeeded(other.bodyBytesNeeded), headerCount(other.headerCount),
	  chunkBytesNeeded(other.chunkBytesNeeded)
{
}

HttpRequestParser &HttpRequestParser::operator=(const HttpRequestParser &other)
{
	if (this == &other)
	{
		return *this;
	}
	state = other.state;
	buffer = other.buffer;
	request = other.request;
	errorStatusCode = other.errorStatusCode;
	maxBodySize = other.maxBodySize;
	bodyBytesNeeded = other.bodyBytesNeeded;
	headerCount = other.headerCount;
	chunkBytesNeeded = other.chunkBytesNeeded;
	return *this;
}

HttpRequestParser::~HttpRequestParser(void)
{
}

std::size_t HttpRequestParser::getMinimumConfirmedLineLength(
	const std::string &str, std::size_t terminatorPos)
{
	if (terminatorPos != std::string::npos)
	{
		return terminatorPos;
	}
	// NOTE: a trailing CR may be the first half of a terminating CRLF
	// rather than part of the body. This check accounts for that
	if (!str.empty() && str[str.size() - 1] == '\r')
	{
		return str.size() - 1;
	}
	return str.size();
}

std::vector<std::string> HttpRequestParser::splitOnCommas(
	const std::string &str)
{
	std::vector<std::string> elements;
	std::size_t start = 0;

	while (true)
	{
		std::size_t separator = str.find(',', start);
		if (separator == std::string::npos)
		{
			elements.push_back(trim(str.substr(start)));
			return elements;
		}
		elements.push_back(trim(str.substr(start, separator - start)));
		start = separator + 1;
	}
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

bool HttpRequestParser::isValidChunkSize(const std::string &str,
                                         std::size_t &out)
{
	static const std::size_t HEX_BASE = 16;
	static const std::size_t DECIMAL_DIGIT_COUNT = 10;

	if (str.empty())
	{
		return false;
	}
	std::size_t value = 0;
	for (std::size_t i = 0; i < str.size(); ++i)
	{
		unsigned char character = static_cast<unsigned char>(str[i]);
		if (!isxdigit(character))
		{
			return false;
		}
		std::size_t digit =
			isdigit(character)
				? static_cast<std::size_t>(character - '0')
				: static_cast<std::size_t>(tolower(character) - 'a') +
					  DECIMAL_DIGIT_COUNT;
		if (value >
		    (std::numeric_limits<std::size_t>::max() - digit) / HEX_BASE)
		{
			return false;
		}
		value = (value * HEX_BASE) + digit;
	}
	out = value;
	return true;
}

void HttpRequestParser::resetInPreparationForNextRequest(void)
{
	state = PARSE_REQUEST_LINE;
	request = HttpRequest();
	errorStatusCode = NO_STATUS_CODE;
	bodyBytesNeeded = 0;
	headerCount = 0;
	chunkBytesNeeded = 0;
}

const HttpRequest &HttpRequestParser::getRequest(void) const
{
	return request;
}

HttpStatusCode HttpRequestParser::getErrorStatusCode(void) const
{
	return errorStatusCode;
}

std::size_t HttpRequestParser::getUnparsedByteCount(void) const
{
	return buffer.size();
}

bool HttpRequestParser::parseRequestLine(void)
{
	static const std::size_t MAX_REQUEST_LINE = 8192;
	std::size_t pos = buffer.find(TERMINATOR);
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

	// RFC 9112 spells the request line as exactly "method SP target SP
	// version". Splitting on the two spaces rather than reading whitespace-
	// separated tokens is what makes a fourth field, a missing field, a run of
	// spaces or a leading space all detectable — extraction with >> would skip
	// and collapse whitespace and then ignore anything after the third token.
	std::size_t firstSpace = line.find(' ');
	std::size_t secondSpace = line.find(' ', firstSpace + 1);
	if (firstSpace == std::string::npos || secondSpace == std::string::npos)
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	std::string methodField = line.substr(0, firstSpace);
	std::string targetField =
		line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
	std::string versionField = line.substr(secondSpace + 1);
	if (methodField.empty() || targetField.empty() || versionField.empty() ||
	    versionField.find(' ') != std::string::npos)
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}

	try
	{
		request.setMethod(
			HttpMethodHelpers::getHttpMethodForString(methodField));
		request.setVersion(
			HttpVersionHelpers::getHttpVersionForString(versionField));
	}
	catch (const std::out_of_range &)
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	if (!HttpVersionHelpers::getWhetherVersionIsSupported(request.getVersion()))
	{
		state = PARSE_ERROR;
		errorStatusCode = HTTP_VERSION_NOT_SUPPORTED;
		return false;
	}
	if (!HttpMethodHelpers::getWhetherMethodIsImplemented(request.getMethod()))
	{
		state = PARSE_ERROR;
		errorStatusCode = NOT_IMPLEMENTED;
		return false;
	}
	request.setPath(targetField);
	state = PARSE_HEADERS;
	return true;
}

bool HttpRequestParser::parseHeaders(void)
{
	static const std::size_t MAX_HEADERS_LINE = 8192;
	std::size_t pos = buffer.find(TERMINATOR);
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
			request.getHeaders().find("host");
		if (request.getVersion() == HTTP_1_1 &&
		    (hostField == request.getHeaders().end() ||
		     hostField->second.empty()))
		{
			state = PARSE_ERROR;
			errorStatusCode = BAD_REQUEST;
			return false;
		}
		return startBody();
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
	if (!request.addHeader(key, val))
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	return true;
}

bool HttpRequestParser::startBody(void)
{
	std::map<std::string, std::string>::const_iterator transferEncodingField =
		request.getHeaders().find("transfer-encoding");
	std::map<std::string, std::string>::const_iterator contentLengthField =
		request.getHeaders().find("content-length");

	if (transferEncodingField != request.getHeaders().end())
	{
		if (contentLengthField != request.getHeaders().end())
		{
			state = PARSE_ERROR;
			errorStatusCode = BAD_REQUEST;
			return false;
		}
		std::vector<std::string> codingNames =
			splitOnCommas(transferEncodingField->second);
		std::vector<TransferEncoding> encodings;
		for (std::size_t i = 0; i < codingNames.size(); ++i)
		{
			try
			{
				encodings.push_back(
					TransferEncodingHelpers::getTransferEncodingForString(
						codingNames[i]));
			}
			catch (const std::out_of_range &)
			{
				state = PARSE_ERROR;
				errorStatusCode = BAD_REQUEST;
				return false;
			}
		}
		if (encodings.size() != 1 ||
		    !TransferEncodingHelpers::getWhetherTransferEncodingIsSupported(
				encodings[0]))
		{
			state = PARSE_ERROR;
			errorStatusCode = NOT_IMPLEMENTED;
			return false;
		}
		if (request.getVersion() != HTTP_1_1)
		{
			state = PARSE_ERROR;
			errorStatusCode = BAD_REQUEST;
			return false;
		}
		state = PARSE_CHUNK_SIZE;
		return true;
	}
	if (contentLengthField != request.getHeaders().end())
	{
		if (!isValidContentLength(contentLengthField->second, bodyBytesNeeded))
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

// A chunked body has no length to check up front, so the limit is applied as
// each chunk lands instead.
bool HttpRequestParser::appendToBodyWithinLimit(const std::string &data)
{
	if (request.getBody().size() + data.size() > maxBodySize)
	{
		state = PARSE_ERROR;
		errorStatusCode = CONTENT_TOO_LARGE;
		return false;
	}
	request.appendToBody(data);
	return true;
}

bool HttpRequestParser::parseChunkSize(void)
{
	static const std::size_t MAX_CHUNK_SIZE_LINE = 8192;
	std::size_t pos = buffer.find(TERMINATOR);
	std::size_t minimumConfirmedLineLength =
		getMinimumConfirmedLineLength(buffer, pos);
	if (minimumConfirmedLineLength > MAX_CHUNK_SIZE_LINE)
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	if (pos == std::string::npos)
	{
		return false;
	}
	std::string line = buffer.substr(0, pos);
	buffer.erase(0, pos + 2);
	// NOTE: chunk extensions are not supported. A slight violation of RFC 9112
	// in favour of simplicity and not having to consider malicious uses that
	// could slow down the server
	if (line.find(';') != std::string::npos)
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	if (!isValidChunkSize(line, chunkBytesNeeded))
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	if (chunkBytesNeeded > maxBodySize - request.getBody().size())
	{
		state = PARSE_ERROR;
		errorStatusCode = CONTENT_TOO_LARGE;
		return false;
	}
	if (chunkBytesNeeded >
	    std::numeric_limits<std::size_t>::max() - TERMINATOR.length())
	{
		state = PARSE_ERROR;
		errorStatusCode = CONTENT_TOO_LARGE;
		return false;
	}
	state = chunkBytesNeeded == 0 ? PARSE_TRAILERS : PARSE_CHUNK_DATA;
	return true;
}

bool HttpRequestParser::parseChunkData(void)
{
	// The chunk's bytes are followed by their own CRLF, which is framing
	// rather than content and has to be present before the chunk is complete.
	if (buffer.size() < chunkBytesNeeded + 2)
	{
		return false;
	}
	if (buffer.compare(chunkBytesNeeded, TERMINATOR.length(), TERMINATOR) != 0)
	{
		state = PARSE_ERROR;
		errorStatusCode = BAD_REQUEST;
		return false;
	}
	if (!appendToBodyWithinLimit(buffer.substr(0, chunkBytesNeeded)))
	{
		return false;
	}
	buffer.erase(0, chunkBytesNeeded + 2);
	chunkBytesNeeded = 0;
	state = PARSE_CHUNK_SIZE;
	return true;
}

bool HttpRequestParser::parseTrailers(void)
{
	static const std::size_t MAX_TRAILER_LINE = 8192;
	std::size_t pos = buffer.find(TERMINATOR);
	std::size_t minimumConfirmedLineLength =
		getMinimumConfirmedLineLength(buffer, pos);
	if (minimumConfirmedLineLength > MAX_TRAILER_LINE)
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
		state = PARSE_DONE;
		return true;
	}
	// Trailer fields are checked for well-formedness and then discarded. They
	// are not merged into the header section: a trailer arrives after the
	// point where the headers have already been acted on, so honouring one
	// would let a sender revise decisions that have already been made.
	std::size_t sep = line.find(":");
	if (sep == std::string::npos || !isValidFieldName(line.substr(0, sep)))
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
	request.appendToBody(buffer.substr(0, bodyBytesNeeded));
	buffer.erase(0, bodyBytesNeeded);
	state = PARSE_DONE;
	return true;
}

HttpRequestParser::Result HttpRequestParser::feed(const char *data,
                                                  std::size_t len)
{
	if (state == PARSE_ERROR)
	{
		return INVALID;
	}
	buffer.append(data, len);
	if (state == PARSE_DONE)
	{
		return COMPLETE;
	}
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
		else if (state == PARSE_CHUNK_SIZE)
		{
			progress = parseChunkSize();
		}
		else if (state == PARSE_CHUNK_DATA)
		{
			progress = parseChunkData();
		}
		else if (state == PARSE_TRAILERS)
		{
			progress = parseTrailers();
		}
	}
	if (state == PARSE_DONE)
	{
		return COMPLETE;
	}
	if (state == PARSE_ERROR)
	{
		return INVALID;
	}
	return INCOMPLETE;
}
