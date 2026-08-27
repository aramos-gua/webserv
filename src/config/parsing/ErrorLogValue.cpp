/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorLogValue.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 11:15:09 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 11:15:25 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorLogValue.hpp"

ErrorLogValue::ErrorLogValue(void)
	: logLevel(DEFAULT_LOG_LEVEL), logLevelSpecified(false)
{
}

ErrorLogValue::ErrorLogValue(const std::string &filePath)
	: filePath(filePath), logLevel(DEFAULT_LOG_LEVEL), logLevelSpecified(false)
{
}

ErrorLogValue::ErrorLogValue(const std::string &filePath, LogLevel logLevel)
	: filePath(filePath), logLevel(logLevel), logLevelSpecified(true)
{
}

ErrorLogValue::ErrorLogValue(const ErrorLogValue &other)
	: filePath(other.filePath), logLevel(other.logLevel),
	  logLevelSpecified(other.logLevelSpecified)
{
}

ErrorLogValue &ErrorLogValue::operator=(const ErrorLogValue &other)
{
	if (this == &other)
	{
		return *this;
	}
	filePath = other.filePath;
	logLevel = other.logLevel;
	logLevelSpecified = other.logLevelSpecified;
	return *this;
}

ErrorLogValue::~ErrorLogValue(void)
{
}

const std::string &ErrorLogValue::getFilePath(void) const
{
	return filePath;
}

LogLevel ErrorLogValue::getLogLevel(void) const
{
	return logLevel;
}

bool ErrorLogValue::getWhetherLogLevelSpecified(void) const
{
	return logLevelSpecified;
}
