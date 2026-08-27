/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersionHelpers.hpp                                   :+:      :+: :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:36:38 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 16:27:17 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_VERSION_HELPERS_HPP
#define HTTP_VERSION_HELPERS_HPP

#include <map>
#include <set>
#include <string>

#include "HttpVersion.hpp"

class HttpVersionHelpers
{
public:
	static HttpVersion getHttpVersionForString(
		const std::string &versionString);
	static std::string getStringForHttpVersion(HttpVersion version);

	static bool getWhetherVersionIsSupported(HttpVersion version);

private:
	static const std::map<std::string, HttpVersion> HTTP_VERSIONS_FOR_STRINGS;
	static const std::map<HttpVersion, std::string> STRINGS_FOR_HTTP_VERSIONS;
	static const std::set<HttpVersion> SUPPORTED_HTTP_VERSIONS;
};

#endif
