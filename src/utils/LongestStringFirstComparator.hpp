/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LongestStringFirstComparator.hpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 23:29:13 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/31 00:14:30 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HELPERS_HPP
#define LOCATION_HELPERS_HPP

#include <string>

struct LongestStringFirstComparator
{
public:
	bool operator()(const std::string &string1,
	                const std::string &string2) const;
};

#endif
