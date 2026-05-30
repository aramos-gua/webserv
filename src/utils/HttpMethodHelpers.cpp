/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethodHelpers.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:45:30 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/01 05:04:04 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utility>

#include "HttpMethodHelpers.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::pair<std::string, HttpMethod> HTTP_METHOD_STRING_PAIRS[] = {
	std::pair<std::string, HttpMethod>("GET", GET),
	std::pair<std::string, HttpMethod>("HEAD", HEAD),
	std::pair<std::string, HttpMethod>("POST", POST),
	std::pair<std::string, HttpMethod>("PUT", PUT),
	std::pair<std::string, HttpMethod>("PATCH", PATCH),
	std::pair<std::string, HttpMethod>("DELETE", DELETE),
};

const std::map<std::string, HttpMethod> HttpMethodHelpers::
	HTTP_METHODS_FOR_STRINGS(HTTP_METHOD_STRING_PAIRS,
                             HTTP_METHOD_STRING_PAIRS +
                                 (sizeof(HTTP_METHOD_STRING_PAIRS) /
                                  sizeof(std::pair<std::string, HttpMethod>)));

// NOLINTEND(bugprone-throwing-static-initialization)

HttpMethod HttpMethodHelpers::getHttpMethodForString(
	const std::string &methodString)
{
	return HTTP_METHODS_FOR_STRINGS.at(methodString);
}
