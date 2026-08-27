/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 21:10:00 by emflynn          ###   ########.fr       */
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

	// The length of the body whether it is held here or not, which is what
	// Content-Length is computed from.
	std::size_t getBodyLength(void) const;
	bool getWhetherBodyIsExternal(void) const;

	void setVersion(HttpVersion version);
	void setStatusCode(HttpStatusCode statusCode);
	// Refuses a name that is not a valid field name, rather than storing it
	// for the builder to write out. A name carrying a CRLF would otherwise
	// append header lines of the caller's choosing to the response.
	bool setHeader(const std::string &name, const std::string &value);

	// The two bodies are set through separate calls that each clear the other,
	// so a response cannot end up claiming an external length while also
	// holding bytes of its own — the one inconsistency these fields allow.
	void setBody(const std::string &body);
	void setExternalBody(std::size_t length);

private:
	HttpVersion version;
	HttpStatusCode statusCode;
	t_http_headers headers;

	// The body is either held here, or it lives outside the response and only
	// its length is recorded. An external body is sent by whoever owns its file
	// descriptor; the response never holds that descriptor, which is what keeps
	// this a copyable value with nothing to decide about copying.
	std::string body;
	bool bodyIsExternal;
	std::size_t externalBodyLength;
};

#endif
