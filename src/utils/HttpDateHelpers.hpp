/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDateHelpers.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 22:10:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 21:48:41 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_DATE_HELPERS_HPP
#define HTTP_DATE_HELPERS_HPP

#include <string>

class HttpDateHelpers
{
public:
	// The current instant as an IMF-fixdate, the format RFC 9110 requires a
	// sender to use and which is always GMT: "Sun, 06 Nov 1994 08:49:37 GMT".
	// Returns empty if the clock cannot be read, so that a caller omits the
	// field rather than sending a wrong one.
	static std::string getCurrentHttpDate(void);

private:
	// NOTE: fixed English abbreviations according to RFC 9110, not
	// locale-dependent
	static const char *const DAY_NAMES[];
	static const char *const MONTH_NAMES[];

	// std::tm counts years from 1900.
	static const int TM_YEAR_EPOCH = 1900;
};

#endif
