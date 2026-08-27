/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigTypeHelpers.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 19:16:58 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/02 19:17:04 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <stdexcept>
#include <utility>

#include "ConfigTypeHelpers.hpp"
#include "StringBase.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::pair<ConfigType, std::string> CONFIG_TYPE_STRING_PAIRS[] = {
	std::pair<ConfigType, std::string>(MAIN, "main"),
	std::pair<ConfigType, std::string>(EVENTS, "events"),
	std::pair<ConfigType, std::string>(HTTP, "http"),
	std::pair<ConfigType, std::string>(SERVER, "server"),
	std::pair<ConfigType, std::string>(LOCATION, "location"),
	std::pair<ConfigType, std::string>(LIMIT_EXCEPT, "limit_except"),
};

const std::map<ConfigType, std::string> ConfigTypeHelpers::
	STRINGS_FOR_CONFIG_TYPES(CONFIG_TYPE_STRING_PAIRS,
                             CONFIG_TYPE_STRING_PAIRS +
                                 (sizeof(CONFIG_TYPE_STRING_PAIRS) /
                                  sizeof(std::pair<ConfigType, std::string>)));

// NOLINTEND(bugprone-throwing-static-initialization)

const std::string &ConfigTypeHelpers::getStringForConfigType(
	ConfigType configType)
{
	std::map<ConfigType, std::string>::const_iterator iterator =
		STRINGS_FOR_CONFIG_TYPES.find(configType);
	if (iterator == STRINGS_FOR_CONFIG_TYPES.end())
	{
		throw std::out_of_range(StringBase() << "Unknown config type "
		                                     << static_cast<int>(configType));
	}
	return iterator->second;
}
