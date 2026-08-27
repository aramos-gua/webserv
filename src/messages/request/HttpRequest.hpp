/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:26:17 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 17:59:10 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <map>
#include <set>
#include <string>

#include "HttpMethod.hpp"
#include "HttpVersion.hpp"

class HttpRequest
{
public:
	HttpRequest(void);
	HttpRequest(const HttpRequest &other);
	HttpRequest &operator=(const HttpRequest &other);
	~HttpRequest(void);

	HttpMethod getMethod(void) const;
	const std::string &getPath(void) const;
	HttpVersion getVersion(void) const;
	const std::map<std::string, std::string> &getHeaders(void) const;
	const std::string &getBody(void) const;

	void setMethod(HttpMethod method);
	void setPath(const std::string &path);
	void setVersion(HttpVersion version);

	bool addHeader(const std::string &name, const std::string &value);
	void appendToBody(const std::string &data);

private:
	static const std::set<std::string> SINGLE_OCCURRENCE_HEADER_NAMES;

	HttpMethod method;
	std::string path;
	HttpVersion version;
	std::map<std::string, std::string> headers;
	std::string body;
};

#endif
