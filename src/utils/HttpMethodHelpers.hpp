/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethodHelpers.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:45:36 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:47:50 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_METHOD_HELPERS_HPP
#define HTTP_METHOD_HELPERS_HPP

#include <map>
#include <string>

#include "HttpMethod.hpp"

class HttpMethodHelpers
{
public:
	static HttpMethod getHttpMethodForString(const std::string &methodString);

private:
	static const std::map<std::string, HttpMethod> HTTP_METHODS_FOR_STRINGS;
};

#endif
