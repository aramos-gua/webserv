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

#ifndef LONGEST_STRING_FIRST_COMPARATOR_HPP
#define LONGEST_STRING_FIRST_COMPARATOR_HPP

#include <string>

struct LongestStringFirstComparator
{
public:
	bool operator()(const std::string &string1,
	                const std::string &string2) const;
};

#endif
