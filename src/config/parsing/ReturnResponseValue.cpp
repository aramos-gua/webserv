/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnResponseValue.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 16:18:11 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:13:48 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpStatusCodeHelpers.hpp"
#include "ReturnResponseValue.hpp"
#include "StringBase.hpp"

ReturnResponseValue::ReturnResponseValue(void)
	: httpStatusCode(NONE), responseType(EMPTY)
{
}

ReturnResponseValue::ReturnResponseValue(HttpStatusCode httpStatusCode)
	: httpStatusCode(httpStatusCode), responseType(EMPTY)
{
	if (httpStatusCode != NO_CONTENT &&
	    !HttpStatusCodeHelpers::isClientError(httpStatusCode) &&
	    !HttpStatusCodeHelpers::isServerError(httpStatusCode))
	{
		throw HttpStatusCodeResponseTypeConflictException(
			StringBase() << httpStatusCode
						 << " return response cannot have response type EMPTY");
	}
}

ReturnResponseValue::ReturnResponseValue(
	HttpStatusCode httpStatusCode, ResponseType responseType,
	const std::string &bodyTextOrRedirectUrl)
	: httpStatusCode(httpStatusCode), responseType(responseType),
	  bodyText(responseType == BODY_TEXT ? bodyTextOrRedirectUrl : ""),
	  redirectUrl(responseType == REDIRECT_URL ? bodyTextOrRedirectUrl : "")
{
	if (!HttpStatusCodeHelpers::isRedirection(httpStatusCode))
	{
		throw HttpStatusCodeResponseTypeConflictException(
			StringBase()
			<< httpStatusCode
			<< " return response cannot have response type REDIRECT_URL");
	}
}

ReturnResponseValue::ReturnResponseValue(const std::string &redirectUrl)
	: httpStatusCode(FOUND), responseType(REDIRECT_URL),
	  redirectUrl(redirectUrl)
{
}

ReturnResponseValue::ReturnResponseValue(const ReturnResponseValue &other)
	: httpStatusCode(other.httpStatusCode), responseType(other.responseType),
	  bodyText(other.bodyText), redirectUrl(other.redirectUrl)
{
}

ReturnResponseValue &ReturnResponseValue::operator=(
	const ReturnResponseValue &other)
{
	if (this == &other)
	{
		return *this;
	}
	httpStatusCode = other.httpStatusCode;
	responseType = other.responseType;
	bodyText = other.bodyText;
	redirectUrl = other.redirectUrl;
	return *this;
}

ReturnResponseValue::~ReturnResponseValue(void)
{
}

HttpStatusCode ReturnResponseValue::getHttpStatusCode(void) const
{
	return httpStatusCode;
}

ReturnResponseValue::ResponseType ReturnResponseValue::getResponseType(
	void) const
{
	return responseType;
}

const std::string &ReturnResponseValue::getBodyText(void) const
{
	return bodyText;
}

const std::string &ReturnResponseValue::getRedirectUrl(void) const
{
	return redirectUrl;
}

ReturnResponseValue::HttpStatusCodeResponseTypeConflictException::
	HttpStatusCodeResponseTypeConflictException(const std::string &errorMessage)
	: std::runtime_error(errorMessage)
{
}
