/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:33:06 by aramos            #+#    #+#             */
/*   Updated: 2026/08/25 21:59:05 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include <cstddef>
#include <string>

#include "HttpRequest.hpp"

class HttpRequestParser
{
private:
	static const std::size_t DEFAULT_MAX_BODY_SIZE = 1024UL * 1024UL;

public:
	enum Result
	{
		INCOMPLETE,
		COMPLETE,
		ERROR
	};

	HttpRequestParser(std::size_t maxBodySize = DEFAULT_MAX_BODY_SIZE);
	HttpRequestParser(const HttpRequestParser &other);
	HttpRequestParser &operator=(const HttpRequestParser &other);
	~HttpRequestParser(void);

	Result feed(const char *data, std::size_t len);
	const HttpRequest &getRequest(void) const;
	const std::string &getError(void) const;
	void reset(void);

	static HttpRequest parse(const std::string &rawRequest);

private:
	enum State
	{
		PARSE_REQUEST_LINE,
		PARSE_HEADERS,
		PARSE_BODY,
		PARSE_DONE,
		PARSE_ERROR
	};

	State state;
	std::string buffer;
	HttpRequest request;
	std::string error;

	std::size_t maxBodySize;
	std::size_t bodyBytesNeeded;
	std::size_t headerCount;

	bool parseRequestLine(void);
	bool parseHeaders(void);
	bool parseBody(void);

	static std::string trim(const std::string &str);
	static bool isValidContentLength(const std::string &str, std::size_t &out);
};

#endif
