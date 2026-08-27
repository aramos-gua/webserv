/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CaseInsensitiveComparator.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 21:55:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 21:55:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstddef>

#include "CaseInsensitiveComparator.hpp"

bool CaseInsensitiveComparator::operator()(const std::string &string1,
                                           const std::string &string2) const
{
	std::size_t shorterSize =
		string1.size() < string2.size() ? string1.size() : string2.size();

	for (std::size_t i = 0; i < shorterSize; ++i)
	{
		int character1 = tolower(static_cast<unsigned char>(string1[i]));
		int character2 = tolower(static_cast<unsigned char>(string2[i]));
		if (character1 != character2)
		{
			return character1 < character2;
		}
	}
	return string1.size() < string2.size();
}
