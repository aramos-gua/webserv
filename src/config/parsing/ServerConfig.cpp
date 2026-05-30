/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 03:57:29 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 07:54:57 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(void)
{
}

ServerConfig::ServerConfig(const ServerConfig &other): AConfig(other)
{
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this == &other)
	{
		return *this;
	}
	AConfig::operator=(other);
	return *this;
}

ServerConfig::~ServerConfig(void)
{
}

ConfigType ServerConfig::getConfigType(void) const
{
	return SERVER;
}
