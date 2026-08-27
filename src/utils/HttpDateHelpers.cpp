/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDateHelpers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 22:10:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 22:10:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <iomanip>
#include <sstream>

#include "HttpDateHelpers.hpp"

const char *const HttpDateHelpers::DAY_NAMES[] = {"Sun", "Mon", "Tue", "Wed",
                                                  "Thu", "Fri", "Sat"};

const char *const HttpDateHelpers::MONTH_NAMES[] = {"Jan", "Feb", "Mar", "Apr",
                                                    "May", "Jun", "Jul", "Aug",
                                                    "Sep", "Oct", "Nov", "Dec"};

std::string HttpDateHelpers::getCurrentHttpDate(void)
{
	std::time_t now = std::time(NULL);
	std::tm *utc = std::gmtime(&now);

	if (utc == NULL)
	{
		return "";
	}
	std::ostringstream out;
	out << DAY_NAMES[utc->tm_wday] << ", " << std::setfill('0') << std::setw(2)
		<< utc->tm_mday << " " << MONTH_NAMES[utc->tm_mon] << " "
		<< (utc->tm_year + TM_YEAR_EPOCH) << " " << std::setw(2) << utc->tm_hour
		<< ":" << std::setw(2) << utc->tm_min << ":" << std::setw(2)
		<< utc->tm_sec << " GMT";
	return out.str();
}
