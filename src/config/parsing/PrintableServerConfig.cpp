/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintableServerConfig.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 02:41:19 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/23 15:18:28 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utility>

#include "PrintableServerConfig.hpp"

PrintableServerConfig::PrintableServerConfig(void)
{
}

PrintableServerConfig::PrintableServerConfig(const PrintableServerConfig &other)
	: defaultServerFlagsByAddressPortPair(
		  other.defaultServerFlagsByAddressPortPair),
	  serverNames(other.serverNames)
{
}

PrintableServerConfig &PrintableServerConfig::operator=(
	const PrintableServerConfig &other)
{
	if (this == &other)
	{
		return *this;
	}
	defaultServerFlagsByAddressPortPair =
		other.defaultServerFlagsByAddressPortPair;
	serverNames = other.serverNames;
	return *this;
}

PrintableServerConfig::~PrintableServerConfig(void)
{
}

// NOTE: if an address-port pair is already present, std::map's insert
// method will not overwrite it, so it will keep its original value for
// the default server flag
void PrintableServerConfig::addAddressPortPair(
	const std::string &addressPortPair)
{
	defaultServerFlagsByAddressPortPair.insert(
		std::pair<std::string, bool>(addressPortPair, false));
}

void PrintableServerConfig::setAddressPortPairAsDefaultServer(
	const std::string &addressPortPair)
{
	defaultServerFlagsByAddressPortPair[addressPortPair] = true;
}

void PrintableServerConfig::addServerName(const std::string &serverName)
{
	serverNames.insert(serverName);
}

const std::map<std::string, bool> &PrintableServerConfig::
	getDefaultServerFlagsByAddressPortPair(void) const
{
	return defaultServerFlagsByAddressPortPair;
}

const std::set<std::string> &PrintableServerConfig::getServerNames(void) const
{
	return serverNames;
}
