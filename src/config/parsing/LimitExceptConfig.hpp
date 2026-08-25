/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptConfig.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:32:18 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:33:26 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMIT_EXCEPT_CONFIG_HPP
#define LIMIT_EXCEPT_CONFIG_HPP

#include "AConfig.hpp"

class LimitExceptConfig: public AConfig
{
public:
	LimitExceptConfig(void);
	LimitExceptConfig(const LimitExceptConfig &other);
	LimitExceptConfig &operator=(const LimitExceptConfig &other);
	~LimitExceptConfig(void);

protected:
	ConfigType getConfigType(void) const;
};

#endif
