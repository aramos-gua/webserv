/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 02:42:48 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/03 16:48:01 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>

#include "HttpConfig.hpp"
#include "MainConfig.hpp"
#include "StringBase.hpp"

MainConfig::MainConfig(void): isFrozen(false)
{
}

MainConfig::MainConfig(const MainConfig &other)
	: AConfig(other), isFrozen(other.isFrozen)
{
}

MainConfig &MainConfig::operator=(const MainConfig &other)
{
	if (this == &other)
	{
		return *this;
	}
	AConfig::operator=(other);
	isFrozen = other.isFrozen;
	return *this;
}

MainConfig::~MainConfig(void)
{
}

void MainConfig::freeze(const std::string &configFilePath)
{
	std::string locationPrefix = StringBase()
	                             << "\"" << configFilePath << "\": ";
	if (!getWhetherEventsConfigSet())
	{
		throw std::runtime_error(locationPrefix +
		                         "\"events\" block is required");
	}
	if (!getWhetherHttpConfigSet())
	{
		throw std::runtime_error(locationPrefix + "\"http\" block is required");
	}
	if (!getHttpConfig().getWhetherAnyServerConfigsSet())
	{
		throw std::runtime_error(
			locationPrefix +
			"\"server\" block is required in \"http\" context");
	}
	isFrozen = true;
}

void MainConfig::throwIfFrozen(void) const
{
	if (isFrozen)
	{
		throw ConfigFrozenException();
	}
}

ConfigType MainConfig::getConfigType(void) const
{
	return MAIN;
}
