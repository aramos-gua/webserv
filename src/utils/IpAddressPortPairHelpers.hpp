/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpAddressPortPairHelpers.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 07:52:28 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/02 09:11:29 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IP_ADDRESS_PORT_PAIR_HELPERS_HPP
#define IP_ADDRESS_PORT_PAIR_HELPERS_HPP

#include <netinet/in.h>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

class IpAddressPortPairHelpers
{
public:
	static const uint16_t DEFAULT_HTTP_PORT = 80;

	static std::vector<std::string> getCanonicalIpAddressPortPairs(
		const std::string &rawAddressPortPair);
	static std::pair<std::string, uint16_t> splitCanonicalIpAddressPortPair(
		const std::string &canonicalAddressPortPair);

private:
	static uint16_t parsePort(const std::string &portAsString);
	static std::pair<std::string, uint16_t> parseBracketedIpV6AddressAndPort(
		const std::string &rawAddressPortPair);
	static std::pair<std::string, uint16_t> parseBareAddressOrPort(
		const std::string &rawAddressPortPair);
	static std::pair<std::string, uint16_t> parseAddressAndPort(
		const std::string &rawAddressPortPair);
	static std::vector<std::string> getAllInterfacesCanonicalIpAddressPortPairs(
		const std::string &port);
	static std::vector<std::string> getIpV4CanonicalIpAddressPortPairs(
		const struct in_addr &addr, const std::string &port);
	static std::vector<std::string> getIpV6CanonicalIpAddressPortPairs(
		const struct in6_addr &addr, const std::string &port);
	static std::vector<std::string> getHostnameCanonicalIpAddressPortPairs(
		const std::string &address, const std::string &port);
};

#endif
