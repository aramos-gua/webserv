/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 03:08:03 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 07:55:42 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_CONFIG_HPP
#define HTTP_CONFIG_HPP

#include "AConfig.hpp"

class HttpConfig: public AConfig
{
public:
	HttpConfig(void);
	HttpConfig(const HttpConfig &other);
	HttpConfig &operator=(const HttpConfig &other);
	~HttpConfig(void);

protected:
	ConfigType getConfigType(void) const;
};

#endif
