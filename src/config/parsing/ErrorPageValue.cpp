/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageValue.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 03:54:31 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 08:30:09 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageValue.hpp"

ErrorPageValue::ErrorPageValue(void): httpStatusCodeToReturn(NONE)
{
}

ErrorPageValue::ErrorPageValue(HttpStatusCode httpStatusCodeToReturn,
                               const std::string &uri)
	: httpStatusCodeToReturn(httpStatusCodeToReturn), uri(uri)
{
}

ErrorPageValue::ErrorPageValue(const ErrorPageValue &other)
	: httpStatusCodeToReturn(other.httpStatusCodeToReturn), uri(other.uri)
{
}

ErrorPageValue &ErrorPageValue::operator=(const ErrorPageValue &other)
{
	if (this == &other)
	{
		return *this;
	}
	httpStatusCodeToReturn = other.httpStatusCodeToReturn;
	uri = other.uri;
	return *this;
}

ErrorPageValue::~ErrorPageValue(void)
{
}

HttpStatusCode ErrorPageValue::getHttpStatusCodeToReturn(void) const
{
	return httpStatusCodeToReturn;
}

const std::string &ErrorPageValue::getUri(void) const
{
	return uri;
}
