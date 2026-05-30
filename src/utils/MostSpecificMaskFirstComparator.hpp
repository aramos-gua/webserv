/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MostSpecificMaskFirstComparator.hpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 03:29:45 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/01 07:41:39 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOST_SPECIFIC_MASK_FIRST_COMPARATOR_HPP
#define MOST_SPECIFIC_MASK_FIRST_COMPARATOR_HPP

#include <cstddef>
#include <string>

struct MostSpecificMaskFirstComparator
{
public:
	bool operator()(const std::string &ipAddressAndMask1,
	                const std::string &ipAddressAndMask2) const;

private:
	static std::size_t getFixedBitCount(const std::string &ipAddressAndMask);
};

#endif
