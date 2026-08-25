/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpAddressPortPairHelpers.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 07:53:11 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/25 08:41:05 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <cstring>
#include <limits>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>

#include "IpAddressPortPairHelpers.hpp"
#include "StringBase.hpp"
#include "StringHelpers.hpp"

const uint16_t IpAddressPortPairHelpers::DEFAULT_HTTP_PORT;

std::vector<std::string> IpAddressPortPairHelpers::
	getCanonicalIpAddressPortPairs(const std::string &rawAddressPortPair)
{
	std::pair<std::string, uint16_t> addressAndPort =
		parseAddressAndPort(rawAddressPortPair);
	const std::string &address = addressAndPort.first;
	std::string port = StringBase() << addressAndPort.second;
	if (address.empty() || address == "*")
	{
		return getAllInterfacesCanonicalIpAddressPortPairs(port);
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in_addr addr4;
	if (inet_pton(AF_INET, address.c_str(), &addr4) == 1)
	{
		return getIpV4CanonicalIpAddressPortPairs(addr4, port);
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct in6_addr addr6;
	if (inet_pton(AF_INET6, address.c_str(), &addr6) == 1)
	{
		return getIpV6CanonicalIpAddressPortPairs(addr6, port);
	}
	return getHostnameCanonicalIpAddressPortPairs(address, port);
}

std::pair<std::string, uint16_t> IpAddressPortPairHelpers::
	splitCanonicalIpAddressPortPair(const std::string &canonicalAddressPortPair)
{
	return parseAddressAndPort(canonicalAddressPortPair);
}

uint16_t IpAddressPortPairHelpers::parsePort(const std::string &portAsString)
{
	static const std::string MAX_PORT_AS_STRING =
		StringBase() << std::numeric_limits<uint16_t>::max();
	if (portAsString.empty())
	{
		throw std::invalid_argument("Port must not be empty");
	}
	if (portAsString.size() > 1 && portAsString[0] == '0')
	{
		throw std::invalid_argument(StringBase()
		                            << "Invalid port \"" << portAsString
		                            << "\": leading zeros not permitted");
	}
	if (!StringHelpers::isAllDigits(portAsString))
	{
		throw std::invalid_argument(StringBase()
		                            << "Invalid port \"" << portAsString
		                            << "\": not a non-negative integer");
	}
	if (portAsString.size() > MAX_PORT_AS_STRING.size())
	{
		throw std::invalid_argument(
			StringBase() << "Invalid port \"" << portAsString
						 << "\": not in range 0 to " << MAX_PORT_AS_STRING);
	}
	std::istringstream stream(portAsString);
	unsigned long port;
	stream >> port;
	if (port > std::numeric_limits<uint16_t>::max())
	{
		throw std::invalid_argument(
			StringBase() << "Invalid port \"" << portAsString
						 << "\": not in range 0 to " << MAX_PORT_AS_STRING);
	}
	return static_cast<uint16_t>(port);
}

std::pair<std::string, uint16_t> IpAddressPortPairHelpers::
	parseBracketedIpV6AddressAndPort(const std::string &rawAddressPortPair)
{
	std::size_t closingBracketPos = rawAddressPortPair.find(']');
	if (closingBracketPos == std::string::npos)
	{
		throw std::invalid_argument(StringBase() << "Invalid listen address \""
		                                         << rawAddressPortPair
		                                         << "\": missing closing ']'");
	}
	std::string address = rawAddressPortPair.substr(1, closingBracketPos - 1);
	uint16_t port = DEFAULT_HTTP_PORT;
	if (closingBracketPos + 1 < rawAddressPortPair.size() &&
	    rawAddressPortPair[closingBracketPos + 1] == ':')
	{
		port = parsePort(rawAddressPortPair.substr(closingBracketPos + 2));
	}
	return std::make_pair(address, port);
}

std::pair<std::string, uint16_t> IpAddressPortPairHelpers::
	parseBareAddressOrPort(const std::string &rawAddressPortPair)
{
	if (StringHelpers::isAllDigits(rawAddressPortPair))
	{
		return std::make_pair(std::string("*"), parsePort(rawAddressPortPair));
	}
	return std::make_pair(rawAddressPortPair, DEFAULT_HTTP_PORT);
}

std::pair<std::string, uint16_t> IpAddressPortPairHelpers::parseAddressAndPort(
	const std::string &rawAddressPortPair)
{
	if (!rawAddressPortPair.empty() && rawAddressPortPair[0] == '[')
	{
		return parseBracketedIpV6AddressAndPort(rawAddressPortPair);
	}
	std::size_t colonCount = 0;
	for (std::size_t i = 0; i < rawAddressPortPair.size(); ++i)
	{
		if (rawAddressPortPair[i] == ':')
		{
			++colonCount;
		}
	}
	if (colonCount > 1)
	{
		throw std::invalid_argument(
			StringBase()
			<< "Invalid listen address \"" << rawAddressPortPair
			<< "\": IPv6 addresses must be enclosed in square brackets");
	}
	if (colonCount == 1)
	{
		std::size_t colonPos = rawAddressPortPair.find(':');
		return std::make_pair(
			rawAddressPortPair.substr(0, colonPos),
			parsePort(rawAddressPortPair.substr(colonPos + 1)));
	}
	return parseBareAddressOrPort(rawAddressPortPair);
}

std::vector<std::string> IpAddressPortPairHelpers::
	getAllInterfacesCanonicalIpAddressPortPairs(const std::string &port)
{
	std::vector<std::string> canonicalIpAddressPortPairs;
	canonicalIpAddressPortPairs.push_back("0.0.0.0:" + port);
	canonicalIpAddressPortPairs.push_back("[::]:" + port);
	return canonicalIpAddressPortPairs;
}

std::vector<std::string> IpAddressPortPairHelpers::
	getIpV4CanonicalIpAddressPortPairs(const struct in_addr &addr,
                                       const std::string &port)
{
	char canonicalisedAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr, canonicalisedAddress, INET_ADDRSTRLEN);
	std::vector<std::string> canonicalIpAddressPortPairs;
	canonicalIpAddressPortPairs.push_back(StringBase() << canonicalisedAddress
	                                                   << ":" << port);
	return canonicalIpAddressPortPairs;
}

std::vector<std::string> IpAddressPortPairHelpers::
	getIpV6CanonicalIpAddressPortPairs(const struct in6_addr &addr,
                                       const std::string &port)
{
	char canonicalisedAddress[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, &addr, canonicalisedAddress, INET6_ADDRSTRLEN);
	std::vector<std::string> canonicalIpAddressPortPairs;
	canonicalIpAddressPortPairs.push_back(
		StringBase() << "[" << canonicalisedAddress << "]:" << port);
	return canonicalIpAddressPortPairs;
}

std::vector<std::string> IpAddressPortPairHelpers::
	getHostnameCanonicalIpAddressPortPairs(const std::string &address,
                                           const std::string &port)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	struct addrinfo hints;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo *addrInfoList;
	int returnCode =
		getaddrinfo(address.c_str(), port.c_str(), &hints, &addrInfoList);
	if (returnCode != 0)
	{
		throw std::invalid_argument(StringBase()
		                            << "Could not resolve address \"" << address
		                            << "\": " << gai_strerror(returnCode));
	}
	std::vector<std::string> canonicalIpAddressPortPairs;
	for (struct addrinfo *addrInfo = addrInfoList; addrInfo != NULL;
	     addrInfo = addrInfo->ai_next)
	{
		if (addrInfo->ai_family == AF_INET)
		{
			char buf[INET_ADDRSTRLEN];
			struct sockaddr_in *socketAddress =
				// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
				reinterpret_cast<struct sockaddr_in *>(addrInfo->ai_addr);
			inet_ntop(AF_INET, &socketAddress->sin_addr, buf, INET_ADDRSTRLEN);
			canonicalIpAddressPortPairs.push_back(StringBase()
			                                      << buf << ":" << port);
		}
		else if (addrInfo->ai_family == AF_INET6)
		{
			char buf[INET6_ADDRSTRLEN];
			struct sockaddr_in6 *socketAddress =
				// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
				reinterpret_cast<struct sockaddr_in6 *>(addrInfo->ai_addr);
			inet_ntop(AF_INET6, &socketAddress->sin6_addr, buf,
			          INET6_ADDRSTRLEN);
			canonicalIpAddressPortPairs.push_back(StringBase() << "[" << buf
			                                                   << "]:" << port);
		}
	}
	freeaddrinfo(addrInfoList);
	if (canonicalIpAddressPortPairs.empty())
	{
		throw std::invalid_argument(
			StringBase() << "Could not resolve address \"" << address
						 << "\": no supported address families found");
	}
	return canonicalIpAddressPortPairs;
}
