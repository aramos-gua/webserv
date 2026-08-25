/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MostSpecificMaskFirstComparator.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 03:30:25 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/01 07:33:01 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

#include "IpAddressHelpers.hpp"
#include "MostSpecificMaskFirstComparator.hpp"

bool MostSpecificMaskFirstComparator::operator()(
	const std::string &ipAddressAndMask1,
	const std::string &ipAddressAndMask2) const
{
	return getFixedBitCount(ipAddressAndMask1) >
	       getFixedBitCount(ipAddressAndMask2);
}

std::size_t MostSpecificMaskFirstComparator::getFixedBitCount(
	const std::string &ipAddressAndMask)
{
	bool maskIsIpV6 = ipAddressAndMask.find_first_of(':') != std::string::npos;
	std::size_t maskFixedBitCount = maskIsIpV6
	                                    ? IpAddressHelpers::IP_V6_BIT_COUNT
	                                    : IpAddressHelpers::IP_V4_BIT_COUNT;
	std::size_t maskSlashPos = ipAddressAndMask.find_last_of('/');
	if (maskSlashPos != std::string::npos)
	{
		std::istringstream(ipAddressAndMask.substr(maskSlashPos + 1)) >>
			maskFixedBitCount;
	}
	return maskFixedBitCount;
}
