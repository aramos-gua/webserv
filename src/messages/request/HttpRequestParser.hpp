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

#include <string>

#include "HttpRequest.hpp"

class HttpRequestParser
{
public:
	enum Result
	{
		INCOMPLETE,
		COMPLETE,
		ERROR
	};

	HttpRequestParser(size_t maxBodySize = 1024 * 1024);
	HttpRequestParser(const HttpRequestParser &other);
	HttpRequestParser &operator=(const HttpRequestParser &other);
	~HttpRequestParser();

	Result feed(const char *data, size_t len);
	const HttpRequest &getRequest() const;
	const std::string &getError() const;
	void reset();

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

	State _state;
	std::string _buffer;
	HttpRequest _req;
	std::string _error;

	size_t _maxBodySize;
	size_t _bodyBytesNeeded;
	size_t _headerCount;

	bool parseRequestLine();
	bool parseHeaders();
	bool parseBody();

	static std::string trim(const std::string &s);
	static bool isValidContentLength(const std::string &s, size_t &out);
};

#endif
