/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigTypeHelpers.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 19:17:05 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/02 19:17:28 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_TYPE_HELPERS_HPP
#define CONFIG_TYPE_HELPERS_HPP

#include <map>
#include <string>

#include "ConfigType.hpp"

class ConfigTypeHelpers
{
public:
	static const std::string &getStringForConfigType(ConfigType configType);

private:
	static const std::map<ConfigType, std::string> STRINGS_FOR_CONFIG_TYPES;
};

#endif
