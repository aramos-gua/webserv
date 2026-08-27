/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:33:06 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 21:39:22 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_BUILDER_HPP
#define HTTP_RESPONSE_BUILDER_HPP

#include "HttpResponse.hpp"

class HttpResponseBuilder
{
public:
	static std::string build(const HttpResponse &res);
};

#endif
