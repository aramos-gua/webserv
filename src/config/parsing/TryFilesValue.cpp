/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TryFilesValue.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 08:15:24 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 08:34:24 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TryFilesValue.hpp"

TryFilesValue::TryFilesValue(void)
	: fallbackType(URL), fallbackHttpStatusCode(NONE)
{
}

TryFilesValue::TryFilesValue(const std::vector<std::string> &files,
                             const std::string &fallbackUrl)
	: files(files), fallbackType(URL), fallbackUrl(fallbackUrl),
	  fallbackHttpStatusCode(NONE)
{
}

TryFilesValue::TryFilesValue(const std::vector<std::string> &files,
                             HttpStatusCode fallbackHttpStatusCode)
	: files(files), fallbackType(HTTP_STATUS_CODE),
	  fallbackHttpStatusCode(fallbackHttpStatusCode)
{
}

TryFilesValue::TryFilesValue(const TryFilesValue &other)
	: files(other.files), fallbackType(other.fallbackType),
	  fallbackUrl(other.fallbackUrl),
	  fallbackHttpStatusCode(other.fallbackHttpStatusCode)
{
}

TryFilesValue &TryFilesValue::operator=(const TryFilesValue &other)
{
	if (this == &other)
	{
		return *this;
	}
	files = other.files;
	fallbackType = other.fallbackType;
	fallbackUrl = other.fallbackUrl;
	fallbackHttpStatusCode = other.fallbackHttpStatusCode;
	return *this;
}

TryFilesValue::~TryFilesValue(void)
{
}

const std::vector<std::string> &TryFilesValue::getFiles(void) const
{
	return files;
}

TryFilesValue::FallbackType TryFilesValue::getFallbackType(void) const
{
	return fallbackType;
}

const std::string &TryFilesValue::getFallbackUrl(void) const
{
	return fallbackUrl;
}

HttpStatusCode TryFilesValue::getFallbackHttpStatusCode(void) const
{
	return fallbackHttpStatusCode;
}
