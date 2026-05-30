/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventsConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 02:49:59 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 07:57:46 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventsConfig.hpp"

EventsConfig::EventsConfig(void)
{
}

EventsConfig::EventsConfig(const EventsConfig &other): AConfig(other)
{
}

EventsConfig &EventsConfig::operator=(const EventsConfig &other)
{
	if (this == &other)
	{
		return *this;
	}
	AConfig::operator=(other);
	return *this;
}

EventsConfig::~EventsConfig(void)
{
}

ConfigType EventsConfig::getConfigType(void) const
{
	return EVENTS;
}
