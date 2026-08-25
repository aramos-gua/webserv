/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptValue.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:25:51 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:57:50 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExceptConfig.hpp"
#include "LimitExceptValue.hpp"

LimitExceptValue::LimitExceptValue(void): limitExceptConfig(NULL)
{
}

LimitExceptValue::LimitExceptValue(const std::set<HttpMethod> &exemptMethods,
                                   const LimitExceptConfig &limitExceptConfig)
	: exemptMethods(exemptMethods),
	  limitExceptConfig(new LimitExceptConfig(limitExceptConfig))
{
}

LimitExceptValue::LimitExceptValue(const LimitExceptValue &other)
	: exemptMethods(other.exemptMethods),
	  limitExceptConfig(other.limitExceptConfig
                            ? new LimitExceptConfig(*other.limitExceptConfig)
                            : NULL)
{
}

LimitExceptValue &LimitExceptValue::operator=(const LimitExceptValue &other)
{
	if (this == &other)
	{
		return *this;
	}
	exemptMethods = other.exemptMethods;
	delete limitExceptConfig;
	limitExceptConfig = other.limitExceptConfig
	                        ? new LimitExceptConfig(*other.limitExceptConfig)
	                        : NULL;
	return *this;
}

LimitExceptValue::~LimitExceptValue(void)
{
	delete limitExceptConfig;
}

const std::set<HttpMethod> &LimitExceptValue::getExemptMethods(void) const
{
	return exemptMethods;
}

const LimitExceptConfig &LimitExceptValue::getLimitExceptConfig(void) const
{
	return *limitExceptConfig;
}
