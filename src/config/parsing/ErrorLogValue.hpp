/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorLogValue.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 11:15:09 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 11:18:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_LOG_VALUE_HPP
#define ERROR_LOG_VALUE_HPP

#include <string>

#include "LogLevel.hpp"

class ErrorLogValue
{
public:
	ErrorLogValue(void);
	ErrorLogValue(const std::string &filePath);
	ErrorLogValue(const std::string &filePath, LogLevel logLevel);
	ErrorLogValue(const ErrorLogValue &other);
	ErrorLogValue &operator=(const ErrorLogValue &other);
	~ErrorLogValue(void);

	const std::string &getFilePath(void) const;
	LogLevel getLogLevel(void) const;
	bool getWhetherLogLevelSpecified(void) const;

private:
	static const LogLevel DEFAULT_LOG_LEVEL = WARN;

	std::string filePath;
	LogLevel logLevel;
	bool logLevelSpecified;
};

#endif
