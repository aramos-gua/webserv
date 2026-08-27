/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CaseInsensitiveComparator.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 21:55:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 21:55:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CASE_INSENSITIVE_COMPARATOR_HPP
#define CASE_INSENSITIVE_COMPARATOR_HPP

#include <string>

struct CaseInsensitiveComparator
{
public:
	bool operator()(const std::string &string1,
	                const std::string &string2) const;
};

#endif
