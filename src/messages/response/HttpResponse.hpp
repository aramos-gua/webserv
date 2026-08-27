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

#include <map>
#include <string>

#include "HttpStatusCode.hpp"
#include "HttpVersion.hpp"

struct HttpResponse
{
	HttpVersion version;
	HttpStatusCode statusCode;
	std::map<std::string, std::string> headers;
	std::string body;

	HttpResponse(void): statusCode(OK), version(HTTP_1_1)
	{
	}
};

#endif
