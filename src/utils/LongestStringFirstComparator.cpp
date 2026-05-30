/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LongestStringFirstComparator.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 23:29:04 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/31 00:14:38 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LongestStringFirstComparator.hpp"

bool LongestStringFirstComparator::operator()(const std::string &string1,
                                              const std::string &string2) const
{
	return string1.size() > string2.size();
}
