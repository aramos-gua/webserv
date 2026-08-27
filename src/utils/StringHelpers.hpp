/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringHelpers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 11:12:27 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 21:23:18 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_HELPERS_HPP
#define STRING_HELPERS_HPP

#include <string>
#include <vector>

class StringHelpers
{
public:
	static bool isAllDigits(const std::string &str);
	static std::string toLowercase(const std::string &str);
	static std::string trim(const std::string &str);
	static std::vector<std::string> splitOnCommas(const std::string &str);
	static std::string removeLineBreaks(const std::string &str);
};

#endif
