/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventsConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 02:49:41 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:34:17 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_CONFIG_HPP
#define EVENTS_CONFIG_HPP

#include "AConfig.hpp"

class EventsConfig: public AConfig
{
public:
	EventsConfig(void);
	EventsConfig(const EventsConfig &other);
	EventsConfig &operator=(const EventsConfig &other);
	~EventsConfig(void);

protected:
	ConfigType getConfigType(void) const;
};

#endif
