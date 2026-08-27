/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogLevelHelpers.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 11:21:53 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 11:22:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_LEVEL_HELPERS_HPP
#define LOG_LEVEL_HELPERS_HPP

#include <map>
#include <string>

#include "LogLevel.hpp"

class LogLevelHelpers
{
public:
	static LogLevel getLogLevelForString(const std::string &logLevelString);
	static const std::string &getStringForLogLevel(LogLevel logLevel);

private:
	static const std::map<std::string, LogLevel> LOG_LEVELS_FOR_STRINGS;
	static const std::map<LogLevel, std::string> STRINGS_FOR_LOG_LEVELS;
};

#endif
