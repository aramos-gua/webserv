/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 22:21:48 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <cstddef>
#include <string>

#include "HttpHeaders.hpp"
#include "HttpStatusCode.hpp"
#include "HttpVersion.hpp"

class HttpResponse
{
public:
	HttpResponse(void);
	HttpResponse(const HttpResponse &other);
	HttpResponse &operator=(const HttpResponse &other);
	~HttpResponse(void);

	HttpVersion getVersion(void) const;
	HttpStatusCode getStatusCode(void) const;
	const t_http_headers &getHeaders(void) const;
	const std::string &getBody(void) const;

	std::size_t getBodyLength(void) const;
	bool getWhetherBodyIsExternal(void) const;

	void setVersion(HttpVersion version);
	void setStatusCode(HttpStatusCode statusCode);
	bool setHeader(const std::string &name, const std::string &value);

	void setBody(const std::string &body);
	void setExternalBody(std::size_t length);

private:
	static const std::size_t MAX_HEADER_COUNT = 100;
	static const std::size_t MAX_HEADER_LINE_LENGTH = 8192;

	HttpVersion version;
	HttpStatusCode statusCode;
	t_http_headers headers;

	std::string body;
	bool bodyIsExternal;
	std::size_t externalBodyLength;
};

#endif
