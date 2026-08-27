/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/08/25 21:58:51 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <cstddef>
#include <map>
#include <string>

struct HttpRequest
{
	std::string method;
	std::string path;
	std::string version;

	std::map<std::string, std::string> headers;

	std::string body;
	std::size_t contentLength;

	HttpRequest(void): contentLength(0)
	{
	}
};

#endif
