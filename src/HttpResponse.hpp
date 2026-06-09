/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aramos <contact@aramos.dev>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/05/30 12:31:56 by aramos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>
# include <map>

struct HttpResponse
{
	int			statusCode;
	std::string	version;
	std::string	description;

	std::map<std::string, std::string>	headers;

	std::string	body;
	HttpResponse() :
		statusCode(200),
		version("HTTP/1.0"),
		description("OK")
	{}
};

#endif
