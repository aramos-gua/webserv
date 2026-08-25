/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnResponseValue.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 16:18:11 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/23 17:53:56 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpStatusCodeHelpers.hpp"
#include "ReturnResponseValue.hpp"
#include "StringBase.hpp"

namespace
{
	bool canBeReturnedWithNothing(HttpStatusCode httpStatusCode)
	{
		return HttpStatusCodeHelpers::takesNoValue(httpStatusCode) ||
		       HttpStatusCodeHelpers::isClientError(httpStatusCode) ||
		       HttpStatusCodeHelpers::isServerError(httpStatusCode);
	}

	bool canBeReturnedWithBodyText(HttpStatusCode httpStatusCode)
	{
		return !HttpStatusCodeHelpers::takesNoValue(httpStatusCode) &&
		       (HttpStatusCodeHelpers::isSuccess(httpStatusCode) ||
		        HttpStatusCodeHelpers::isClientError(httpStatusCode) ||
		        HttpStatusCodeHelpers::isServerError(httpStatusCode));
	}

	bool canBeReturnedWithRedirectUrl(HttpStatusCode httpStatusCode)
	{
		return HttpStatusCodeHelpers::isRedirection(httpStatusCode) &&
		       httpStatusCode != NOT_MODIFIED;
	}

	bool isValidForResponseType(HttpStatusCode httpStatusCode,
	                            ReturnResponseValue::ResponseType responseType)
	{
		switch (responseType)
		{
		case ReturnResponseValue::EMPTY:
			return canBeReturnedWithNothing(httpStatusCode);
		case ReturnResponseValue::BODY_TEXT:
			return canBeReturnedWithBodyText(httpStatusCode);
		case ReturnResponseValue::REDIRECT_URL:
			return canBeReturnedWithRedirectUrl(httpStatusCode);
		}
		return false;
	}

	const char *getNameForResponseType(
		ReturnResponseValue::ResponseType responseType)
	{
		switch (responseType)
		{
		case ReturnResponseValue::EMPTY:
			return "EMPTY";
		case ReturnResponseValue::BODY_TEXT:
			return "BODY_TEXT";
		case ReturnResponseValue::REDIRECT_URL:
			return "REDIRECT_URL";
		}
		return "";
	}

	void throwIfNotReturnable(HttpStatusCode httpStatusCode,
	                          ReturnResponseValue::ResponseType responseType)
	{
		if (HttpStatusCodeHelpers::isInformational(httpStatusCode))
		{
			throw ReturnResponseValue::InvalidReturnResponseException(
				StringBase() << httpStatusCode
							 << " is an informational status code, which "
								"cannot be returned");
		}
		if (HttpStatusCodeHelpers::takesNoValue(httpStatusCode) &&
		    responseType != ReturnResponseValue::EMPTY)
		{
			throw ReturnResponseValue::InvalidReturnResponseException(
				StringBase()
				<< httpStatusCode << " return response cannot have a value");
		}
		if (!isValidForResponseType(httpStatusCode, responseType))
		{
			throw ReturnResponseValue::InvalidReturnResponseException(
				StringBase() << httpStatusCode
							 << " return response cannot have response type "
							 << getNameForResponseType(responseType));
		}
	}
}

ReturnResponseValue::ReturnResponseValue(void)
	: httpStatusCode(NONE), responseType(EMPTY)
{
}

ReturnResponseValue::ReturnResponseValue(HttpStatusCode httpStatusCode)
	: httpStatusCode(httpStatusCode), responseType(EMPTY)
{
	throwIfNotReturnable(httpStatusCode, EMPTY);
}

ReturnResponseValue::ReturnResponseValue(
	HttpStatusCode httpStatusCode, ResponseType responseType,
	const std::string &bodyTextOrRedirectUrl)
	: httpStatusCode(httpStatusCode), responseType(responseType),
	  bodyText(responseType == BODY_TEXT ? bodyTextOrRedirectUrl : ""),
	  redirectUrl(responseType == REDIRECT_URL ? bodyTextOrRedirectUrl : "")
{
	throwIfNotReturnable(httpStatusCode, responseType);
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

ReturnResponseValue::InvalidReturnResponseException::
	InvalidReturnResponseException(const std::string &errorMessage)
	: std::runtime_error(errorMessage)
{
}
