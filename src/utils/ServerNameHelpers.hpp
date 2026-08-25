/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNameHelpers.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 07:34:16 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/02 07:34:52 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_NAME_HELPERS_HPP
#define SERVER_NAME_HELPERS_HPP

#include <string>

class ServerNameHelpers
{
public:
	static bool isValidServerName(const std::string &serverName);
};

#endif
