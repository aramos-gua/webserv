/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 03:08:53 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 07:56:48 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConfig.hpp"

HttpConfig::HttpConfig(void)
{
}

HttpConfig::HttpConfig(const HttpConfig &other): AConfig(other)
{
}

HttpConfig &HttpConfig::operator=(const HttpConfig &other)
{
	if (this == &other)
	{
		return *this;
	}
	AConfig::operator=(other);
	return *this;
}

HttpConfig::~HttpConfig(void)
{
}

ConfigType HttpConfig::getConfigType(void) const
{
	return HTTP;
}
