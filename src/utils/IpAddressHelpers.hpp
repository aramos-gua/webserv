/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpAddressHelpers.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 07:27:22 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/02 07:13:52 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IP_ADDRESS_HELPERS_HPP
#define IP_ADDRESS_HELPERS_HPP

#include <cstddef>
#include <stdint.h>
#include <string>

class IpAddressHelpers
{
public:
	static const std::size_t BITS_PER_BYTE = 8;
	static const std::size_t IP_V4_BIT_COUNT = 32;
	static const std::size_t IP_V6_BIT_COUNT = 128;
	static const std::size_t IP_V6_BYTE_COUNT = IP_V6_BIT_COUNT / BITS_PER_BYTE;
	static const uint8_t EMPTY_BYTE_MASK = 0x00U;
	static const uint8_t FULL_BYTE_MASK = 0xFFU;

	static bool isValidIpAddressAndMask(const std::string &ipAddressAndMask);
	static std::string canonicaliseIpAddressAndMask(
		const std::string &ipAddressAndMask);
	static bool ipAddressBelongsToSubnet(const std::string &ipAddress,
	                                     const std::string &subnet);

private:
	static bool isValidIpV4AddressAndMask(const std::string &ipAddressAndMask);
	static bool isValidIpV6AddressAndMask(const std::string &ipAddressAndMask);

	static std::string canonicaliseIpV4AddressAndMask(
		const std::string &ipAddressAndMask);
	static std::string canonicaliseIpV6AddressAndMask(
		const std::string &ipAddressAndMask);

	static bool ipV4AddressBelongsToSubnet(const std::string &ipAddress,
	                                       const std::string &subnet);
	static bool ipV6AddressBelongsToSubnet(const std::string &ipAddress,
	                                       const std::string &subnet);

	static uint8_t getIpV6MaskByte(std::size_t byteIndex,
	                               std::size_t prefixLength);
};

#endif
