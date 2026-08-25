/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 03:57:29 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 08:10:50 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

LocationConfig::LocationConfig(void)
{
}

LocationConfig::LocationConfig(const LocationConfig &other): AConfig(other)
{
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this == &other)
	{
		return *this;
	}
	AConfig::operator=(other);
	return *this;
}

LocationConfig::~LocationConfig(void)
{
}

ConfigType LocationConfig::getConfigType(void) const
{
	return LOCATION;
}
