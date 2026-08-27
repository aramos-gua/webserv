/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringHelpers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 11:12:27 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 21:53:35 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_HELPERS_HPP
#define STRING_HELPERS_HPP

#include <cstddef>
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
	static bool parseSize(const std::string &str, std::size_t &out);
};

#endif
