/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethodHelpers.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:45:36 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 16:28:05 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_METHOD_HELPERS_HPP
#define HTTP_METHOD_HELPERS_HPP

#include <map>
#include <set>
#include <string>

#include "HttpMethod.hpp"

class HttpMethodHelpers
{
public:
	static HttpMethod getHttpMethodForString(const std::string &methodString);
	static std::string getStringForHttpMethod(HttpMethod method);

	static bool getWhetherMethodIsImplemented(HttpMethod method);

private:
	static const std::map<std::string, HttpMethod> HTTP_METHODS_FOR_STRINGS;
	static const std::map<HttpMethod, std::string> STRINGS_FOR_HTTP_METHODS;
	static const std::set<HttpMethod> IMPLEMENTED_HTTP_METHODS;
};

#endif
