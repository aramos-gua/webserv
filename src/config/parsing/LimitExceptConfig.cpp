/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptConfig.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:33:48 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:33:59 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExceptConfig.hpp"

LimitExceptConfig::LimitExceptConfig(void)
{
}

LimitExceptConfig::LimitExceptConfig(const LimitExceptConfig &other)
	: AConfig(other)
{
}

LimitExceptConfig &LimitExceptConfig::operator=(const LimitExceptConfig &other)
{
	if (this == &other)
	{
		return *this;
	}
	AConfig::operator=(other);
	return *this;
}

LimitExceptConfig::~LimitExceptConfig(void)
{
}

ConfigType LimitExceptConfig::getConfigType(void) const
{
	return LIMIT_EXCEPT;
}
