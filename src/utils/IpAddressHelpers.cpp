/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpAddressHelpers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 07:27:08 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/01 08:12:41 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <stdint.h>

#include "IpAddressHelpers.hpp"

bool IpAddressHelpers::isValidIpAddressAndMask(
	const std::string &ipAddressAndMask)
{
	if (ipAddressAndMask.find(':') != std::string::npos)
	{
		return isValidIpV6AddressAndMask(ipAddressAndMask);
	}
	return isValidIpV4AddressAndMask(ipAddressAndMask);
}

std::string IpAddressHelpers::canonicaliseIpAddressAndMask(
	const std::string &ipAddressAndMask)
{
	if (ipAddressAndMask.find(':') != std::string::npos)
	{
		return canonicaliseIpV6AddressAndMask(ipAddressAndMask);
	}
	return canonicaliseIpV4AddressAndMask(ipAddressAndMask);
}

bool IpAddressHelpers::ipAddressBelongsToSubnet(const std::string &ipAddress,
                                                const std::string &subnet)
{
	if (subnet.find(':') != std::string::npos)
	{
		return ipV6AddressBelongsToSubnet(ipAddress, subnet);
	}
	return ipV4AddressBelongsToSubnet(ipAddress, subnet);
}

bool IpAddressHelpers::isValidIpV4AddressAndMask(
	const std::string &ipAddressAndMask)
{
	std::size_t maskSlashPos = ipAddressAndMask.find_last_of('/');
	std::string ipAddress = ipAddressAndMask.substr(0, maskSlashPos);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in_addr internetAddress;
	if (inet_pton(AF_INET, ipAddress.c_str(), &internetAddress) != 1)
	{
		return false;
	}
	if (maskSlashPos != std::string::npos)
	{
		std::size_t maskFixedBitCount = IP_V4_BIT_COUNT + 1;
		std::istringstream(ipAddressAndMask.substr(maskSlashPos + 1)) >>
			maskFixedBitCount;
		if (maskFixedBitCount > IP_V4_BIT_COUNT)
		{
			return false;
		}
	}
	return true;
}

bool IpAddressHelpers::isValidIpV6AddressAndMask(
	const std::string &ipAddressAndMask)
{
	std::size_t maskSlashPos = ipAddressAndMask.find_last_of('/');
	std::string ipAddress = ipAddressAndMask.substr(0, maskSlashPos);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in6_addr internetAddress;
	if (inet_pton(AF_INET6, ipAddress.c_str(), &internetAddress) != 1)
	{
		return false;
	}
	if (maskSlashPos != std::string::npos)
	{
		std::size_t maskFixedBitCount = IP_V6_BIT_COUNT + 1;
		std::istringstream(ipAddressAndMask.substr(maskSlashPos + 1)) >>
			maskFixedBitCount;
		if (maskFixedBitCount > IP_V6_BIT_COUNT)
		{
			return false;
		}
	}
	return true;
}

std::string IpAddressHelpers::canonicaliseIpV4AddressAndMask(
	const std::string &ipAddressAndMask)
{
	std::size_t maskSlashPos = ipAddressAndMask.find_last_of('/');
	std::string ipAddress = ipAddressAndMask.substr(0, maskSlashPos);
	std::size_t maskFixedBitCount = IP_V4_BIT_COUNT;
	if (maskSlashPos != std::string::npos)
	{
		std::istringstream(ipAddressAndMask.substr(maskSlashPos + 1)) >>
			maskFixedBitCount;
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in_addr internetAddress;
	inet_pton(AF_INET, ipAddress.c_str(), &internetAddress);
	uint32_t mask = maskFixedBitCount == 0
	                    ? static_cast<uint32_t>(0)
	                    : (~static_cast<uint32_t>(0)
	                       << (IP_V4_BIT_COUNT - maskFixedBitCount));
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in_addr canonicalisedInternetAddress;
	canonicalisedInternetAddress.s_addr =
		htonl(ntohl(internetAddress.s_addr) & mask);
	char canonicalisedIpAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &canonicalisedInternetAddress, canonicalisedIpAddress,
	          INET_ADDRSTRLEN);
	std::ostringstream canonicalisedIpAddressAndMask;
	canonicalisedIpAddressAndMask << canonicalisedIpAddress << '/'
								  << maskFixedBitCount;
	return canonicalisedIpAddressAndMask.str();
}

std::string IpAddressHelpers::canonicaliseIpV6AddressAndMask(
	const std::string &ipAddressAndMask)
{
	std::size_t maskSlashPos = ipAddressAndMask.find_last_of('/');
	std::string ipAddress = ipAddressAndMask.substr(0, maskSlashPos);
	std::size_t maskFixedBitCount = IP_V6_BIT_COUNT;
	if (maskSlashPos != std::string::npos)
	{
		std::istringstream(ipAddressAndMask.substr(maskSlashPos + 1)) >>
			maskFixedBitCount;
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in6_addr internetAddress;
	inet_pton(AF_INET6, ipAddress.c_str(), &internetAddress);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in6_addr canonicalisedInternetAddress;
	for (std::size_t i = 0; i < IP_V6_BYTE_COUNT; ++i)
	{
		canonicalisedInternetAddress.s6_addr[i] =
			internetAddress.s6_addr[i] & getIpV6MaskByte(i, maskFixedBitCount);
	}
	char canonicalisedIpAddress[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, &canonicalisedInternetAddress, canonicalisedIpAddress,
	          INET6_ADDRSTRLEN);
	std::ostringstream canonicalisedIpAddressAndMask;
	canonicalisedIpAddressAndMask << canonicalisedIpAddress << '/'
								  << maskFixedBitCount;
	return canonicalisedIpAddressAndMask.str();
}

bool IpAddressHelpers::ipV4AddressBelongsToSubnet(const std::string &ipAddress,
                                                  const std::string &subnet)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in_addr internetAddress;
	if (inet_pton(AF_INET, ipAddress.c_str(), &internetAddress) != 1)
	{
		return false;
	}
	uint32_t ipAddressAsInt = ntohl(internetAddress.s_addr);
	std::size_t subnetMaskSlashPos = subnet.find_last_of('/');
	std::string subnetIpAddress = subnet.substr(0, subnetMaskSlashPos);
	std::size_t subnetMaskFixedBitCount = IP_V4_BIT_COUNT;
	if (subnetMaskSlashPos != std::string::npos)
	{
		std::istringstream(subnet.substr(subnetMaskSlashPos + 1)) >>
			subnetMaskFixedBitCount;
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in_addr subnetInternetAddress;
	if (inet_pton(AF_INET, subnetIpAddress.c_str(), &subnetInternetAddress) !=
	    1)
	{
		return false;
	}
	uint32_t subnetIpAddressAsInt = ntohl(subnetInternetAddress.s_addr);
	uint32_t subnetMask =
		subnetMaskFixedBitCount == 0
			? static_cast<uint32_t>(0)
			: (~static_cast<uint32_t>(0)
	           << (IP_V4_BIT_COUNT - subnetMaskFixedBitCount));
	return (ipAddressAsInt & subnetMask) == (subnetIpAddressAsInt & subnetMask);
}

bool IpAddressHelpers::ipV6AddressBelongsToSubnet(const std::string &ipAddress,
                                                  const std::string &subnet)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in6_addr internetAddress;
	if (inet_pton(AF_INET6, ipAddress.c_str(), &internetAddress) != 1)
	{
		return false;
	}
	std::size_t subnetMaskSlashPos = subnet.find_last_of('/');
	std::string subnetIpAddress = subnet.substr(0, subnetMaskSlashPos);
	std::size_t subnetMaskFixedBitCount = IP_V6_BIT_COUNT;
	if (subnetMaskSlashPos != std::string::npos)
	{
		std::istringstream(subnet.substr(subnetMaskSlashPos + 1)) >>
			subnetMaskFixedBitCount;
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in6_addr subnetInternetAddress;
	if (inet_pton(AF_INET6, subnetIpAddress.c_str(), &subnetInternetAddress) !=
	    1)
	{
		return false;
	}
	for (std::size_t i = 0; i < IP_V6_BYTE_COUNT; ++i)
	{
		uint8_t maskByte = getIpV6MaskByte(i, subnetMaskFixedBitCount);
		if ((internetAddress.s6_addr[i] & maskByte) !=
		    (subnetInternetAddress.s6_addr[i] & maskByte))
		{
			return false;
		}
	}
	return true;
}

uint8_t IpAddressHelpers::getIpV6MaskByte(std::size_t byteIndex,
                                          std::size_t prefixLength)
{
	std::size_t byteFirstBit = byteIndex * BITS_PER_BYTE;
	if (byteFirstBit + BITS_PER_BYTE <= prefixLength)
	{
		return FULL_BYTE_MASK;
	}
	if (byteFirstBit >= prefixLength)
	{
		return EMPTY_BYTE_MASK;
	}
	return static_cast<uint8_t>(
		(FULL_BYTE_MASK << (BITS_PER_BYTE - (prefixLength - byteFirstBit))) &
		FULL_BYTE_MASK);
}
