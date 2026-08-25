/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpecialServerNamesLastComparator.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 01:12:16 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/23 15:41:01 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SpecialServerNames.hpp"
#include "SpecialServerNamesLastComparator.hpp"

namespace
{
	// Ordinary (non-special) server names determine the order in which servers
	// for a particular address-port pair are printed for testing purposes.
	// Special server names, on the other hand, do not determine printing order
	// (since they are not in the original config file in the same form)
	enum ServerNameRank
	{
		ORDINARY_SERVER_NAME,
		CATCH_ALL_SERVER_NAME,
		DEFAULT_SERVER_NAME,
		FIRST_SERVER_NAME
	};

	ServerNameRank getRankForServerName(const std::string &serverName)
	{
		if (serverName == SpecialServerNames::CATCH_ALL)
		{
			return CATCH_ALL_SERVER_NAME;
		}
		if (serverName == SpecialServerNames::DEFAULT)
		{
			return DEFAULT_SERVER_NAME;
		}
		if (serverName == SpecialServerNames::FIRST)
		{
			return FIRST_SERVER_NAME;
		}
		return ORDINARY_SERVER_NAME;
	}
}

bool SpecialServerNamesLastComparator::operator()(
	const std::string &serverName1, const std::string &serverName2) const
{
	ServerNameRank rank1 = getRankForServerName(serverName1);
	ServerNameRank rank2 = getRankForServerName(serverName2);
	if (rank1 != rank2)
	{
		return rank1 < rank2;
	}
	return serverName1 < serverName2;
}
