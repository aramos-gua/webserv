/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 15:55:37 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <cstddef>
#include <map>
#include <string>

#include "HttpMethod.hpp"

struct HttpRequest
{
	HttpMethod method;
	std::string path;
	std::string version;

	std::map<std::string, std::string> headers;

	std::string body;
	std::size_t contentLength;

	HttpRequest(void): method(NO_METHOD), contentLength(0)
	{
	}
};

#endif
