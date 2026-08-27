/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:33:06 by aramos            #+#    #+#             */
/*   Updated: 2026/08/25 21:59:22 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_BUILDER_HPP
#define HTTP_RESPONSE_BUILDER_HPP

#include "HttpResponse.hpp"

class HttpResponseBuilder
{
private:
	static std::string sanitizeHeaderValue(const std::string &str);

public:
	static std::string build(const HttpResponse &res);
};

#endif
