/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnResponseValue.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 16:18:18 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:11:48 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RETURN_RESPONSE_VALUE
#define RETURN_RESPONSE_VALUE

#include <stdexcept>
#include <string>

#include "HttpStatusCode.hpp"

class ReturnResponseValue
{
public:
	enum ResponseType
	{
		EMPTY,
		BODY_TEXT,
		REDIRECT_URL,
	};

	ReturnResponseValue(void);
	ReturnResponseValue(HttpStatusCode httpStatusCode);
	ReturnResponseValue(HttpStatusCode httpStatusCode,
	                    ResponseType responseType,
	                    const std::string &bodyTextOrRedirectUrl);
	ReturnResponseValue(const std::string &redirectUrl);
	ReturnResponseValue(const ReturnResponseValue &other);
	ReturnResponseValue &operator=(const ReturnResponseValue &other);
	~ReturnResponseValue(void);

	HttpStatusCode getHttpStatusCode(void) const;
	ResponseType getResponseType(void) const;
	const std::string &getBodyText(void) const;
	const std::string &getRedirectUrl(void) const;

	class HttpStatusCodeResponseTypeConflictException: public std::runtime_error
	{
	public:
		HttpStatusCodeResponseTypeConflictException(
			const std::string &errorMessage);
	};

private:
	HttpStatusCode httpStatusCode;
	ResponseType responseType;
	std::string bodyText;
	std::string redirectUrl;
};

#endif
