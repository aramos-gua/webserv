/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogLevelHelpers.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 12:00:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 12:04:10 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <utility>

#include "LogLevelHelpers.hpp"
#include "StringBase.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::pair<std::string, LogLevel> LOG_LEVEL_STRING_PAIRS[] = {
	std::pair<std::string, LogLevel>("fatal", FATAL),
	std::pair<std::string, LogLevel>("error", ERROR),
	std::pair<std::string, LogLevel>("warn", WARN),
	std::pair<std::string, LogLevel>("info", INFO),
	std::pair<std::string, LogLevel>("debug", DEBUG),
};

static const std::pair<LogLevel, std::string> LOG_LEVEL_PAIRS_BY_LEVEL[] = {
	std::pair<LogLevel, std::string>(FATAL, "fatal"),
	std::pair<LogLevel, std::string>(ERROR, "error"),
	std::pair<LogLevel, std::string>(WARN, "warn"),
	std::pair<LogLevel, std::string>(INFO, "info"),
	std::pair<LogLevel, std::string>(DEBUG, "debug"),
};

const std::map<std::string, LogLevel> LogLevelHelpers::LOG_LEVELS_FOR_STRINGS(
	LOG_LEVEL_STRING_PAIRS,
	LOG_LEVEL_STRING_PAIRS + (sizeof(LOG_LEVEL_STRING_PAIRS) /
                              sizeof(std::pair<std::string, LogLevel>)));

const std::map<LogLevel, std::string> LogLevelHelpers::STRINGS_FOR_LOG_LEVELS(
	LOG_LEVEL_PAIRS_BY_LEVEL,
	LOG_LEVEL_PAIRS_BY_LEVEL + (sizeof(LOG_LEVEL_PAIRS_BY_LEVEL) /
                                sizeof(std::pair<LogLevel, std::string>)));

// NOLINTEND(bugprone-throwing-static-initialization)

LogLevel LogLevelHelpers::getLogLevelForString(
	const std::string &logLevelString)
{
	std::map<std::string, LogLevel>::const_iterator iterator =
		LOG_LEVELS_FOR_STRINGS.find(logLevelString);
	if (iterator == LOG_LEVELS_FOR_STRINGS.end())
	{
		throw std::out_of_range(StringBase() << "Unknown log level \""
		                                     << logLevelString << "\"");
	}
	return iterator->second;
}

const std::string &LogLevelHelpers::getStringForLogLevel(LogLevel logLevel)
{
	std::map<LogLevel, std::string>::const_iterator iterator =
		STRINGS_FOR_LOG_LEVELS.find(logLevel);
	if (iterator == STRINGS_FOR_LOG_LEVELS.end())
	{
		throw std::out_of_range(StringBase() << "Unknown log level "
		                                     << static_cast<int>(logLevel));
	}
	return iterator->second;
}
