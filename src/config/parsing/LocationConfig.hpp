/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 03:57:08 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:28:48 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include "AConfig.hpp"

class LocationConfig: public AConfig
{
public:
	LocationConfig(void);
	LocationConfig(const LocationConfig &other);
	LocationConfig &operator=(const LocationConfig &other);
	~LocationConfig(void);

protected:
	ConfigType getConfigType(void) const;
};

#endif
