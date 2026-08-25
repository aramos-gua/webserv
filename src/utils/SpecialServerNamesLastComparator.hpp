/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpecialServerNamesLastComparator.hpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 01:12:04 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/23 01:12:04 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPECIAL_SERVER_NAMES_LAST_COMPARATOR_HPP
#define SPECIAL_SERVER_NAMES_LAST_COMPARATOR_HPP

#include <string>

struct SpecialServerNamesLastComparator
{
public:
	bool operator()(const std::string &serverName1,
	                const std::string &serverName2) const;
};

#endif
