/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 20:20:31 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <cstddef>
#include <map>
#include <string>

#include "HttpStatusCode.hpp"
#include "HttpVersion.hpp"

struct HttpResponse
{
	HttpVersion version;
	HttpStatusCode statusCode;
	std::map<std::string, std::string> headers;

	// The body is either held here, or it lives outside the response and only
	// its length is recorded. An external body is sent by whoever owns its file
	// descriptor; the response needs nothing from it but the length, which is
	// what Content-Length is computed from.
	std::string body;
	bool bodyIsExternal;
	std::size_t externalBodyLength;

	HttpResponse(void)
		: version(HTTP_1_1), statusCode(OK), bodyIsExternal(false),
		  externalBodyLength(0)
	{
	}

	std::size_t getBodyLength(void) const
	{
		return bodyIsExternal ? externalBodyLength : body.size();
	}
};

#endif
