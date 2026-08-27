/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:33:06 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 19:16:15 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include <cstddef>
#include <string>

#include "HttpRequest.hpp"
#include "HttpStatusCode.hpp"

class HttpRequestParser
{
private:
	static const std::size_t DEFAULT_MAX_BODY_SIZE = 1024UL * 1024UL;

public:
	enum Result
	{
		INCOMPLETE,
		COMPLETE,
		INVALID
	};

	HttpRequestParser(std::size_t maxBodySize = DEFAULT_MAX_BODY_SIZE);
	HttpRequestParser(const HttpRequestParser &other);
	HttpRequestParser &operator=(const HttpRequestParser &other);
	~HttpRequestParser(void);

	// NOTE: prefer many smaller calls to one large one. Illustrative example:
	// 32MB in a single call takes around 50x longer than in many 2KB reads.
	Result feed(const char *data, std::size_t len);
	const HttpRequest &getRequest(void) const;
	HttpStatusCode getErrorStatusCode(void) const;
	std::size_t getUnparsedByteCount(void) const;
	void resetInPreparationForNextRequest(void);

private:
	enum State
	{
		PARSE_REQUEST_LINE,
		PARSE_HEADERS,
		PARSE_BODY,
		PARSE_CHUNK_SIZE,
		PARSE_CHUNK_DATA,
		PARSE_TRAILERS,
		PARSE_DONE,
		PARSE_ERROR
	};

	static const std::size_t MAX_LINE_LENGTH = 8192;
	static const std::size_t MAX_HEADER_COUNT = 100;

	State state;
	std::string buffer;
	HttpRequest request;
	HttpStatusCode errorStatusCode;

	std::size_t maxBodySize;
	std::size_t bodyBytesNeeded;
	std::size_t headerCount;
	std::size_t chunkBytesNeeded;

	bool parseRequestLine(void);
	bool parseHeaders(void);
	bool parseBody(void);
	bool parseChunkSize(void);
	bool parseChunkData(void);
	bool parseTrailers(void);

	bool readLine(std::size_t maxLength, HttpStatusCode overflowStatusCode,
	              std::string &line);
	bool startBody(void);
	bool appendToBodyWithinLimit(const std::string &data);

	bool fail(HttpStatusCode statusCode);

	static std::size_t getMinimumConfirmedLineLength(const std::string &str,
	                                                 std::size_t terminatorPos);
	static bool isValidOriginFormTarget(const std::string &target);
	static bool isValidChunkSize(const std::string &str, std::size_t &out);
};

#endif
