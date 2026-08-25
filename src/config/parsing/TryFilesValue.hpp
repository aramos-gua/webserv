/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TryFilesValue.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 08:15:31 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 08:33:12 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRY_FILES_VALUE_HPP
#define TRY_FILES_VALUE_HPP

#include <string>
#include <vector>

#include "HttpStatusCode.hpp"

class TryFilesValue
{
public:
	enum FallbackType
	{
		URL,
		HTTP_STATUS_CODE,
	};

	TryFilesValue(void);
	TryFilesValue(const std::vector<std::string> &files,
	              const std::string &fallbackUrl);
	TryFilesValue(const std::vector<std::string> &files,
	              HttpStatusCode fallbackHttpStatusCode);
	TryFilesValue(const TryFilesValue &other);
	TryFilesValue &operator=(const TryFilesValue &other);
	~TryFilesValue(void);

	const std::vector<std::string> &getFiles(void) const;
	FallbackType getFallbackType(void) const;
	const std::string &getFallbackUrl(void) const;
	HttpStatusCode getFallbackHttpStatusCode(void) const;

private:
	std::vector<std::string> files;
	FallbackType fallbackType;
	std::string fallbackUrl;
	HttpStatusCode fallbackHttpStatusCode;
};

#endif
