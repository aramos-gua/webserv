/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintableServerConfig.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 02:41:08 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/23 15:05:38 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTABLE_SERVER_CONFIG_HPP
#define PRINTABLE_SERVER_CONFIG_HPP

#include <map>
#include <set>
#include <string>

class PrintableServerConfig
{
public:
	PrintableServerConfig(void);
	PrintableServerConfig(const PrintableServerConfig &other);
	PrintableServerConfig &operator=(const PrintableServerConfig &other);
	~PrintableServerConfig(void);

	void addAddressPortPair(const std::string &addressPortPair);
	void setAddressPortPairAsDefaultServer(const std::string &addressPortPair);
	void addServerName(const std::string &serverName);

	const std::map<std::string, bool> &getDefaultServerFlagsByAddressPortPair(
		void) const;
	const std::set<std::string> &getServerNames(void) const;

private:
	std::map<std::string, bool> defaultServerFlagsByAddressPortPair;
	std::set<std::string> serverNames;
};

#endif
