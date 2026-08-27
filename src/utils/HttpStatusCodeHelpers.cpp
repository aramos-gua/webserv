/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCodeHelpers.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 16:55:42 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/23 17:44:28 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <utility>

#include "HttpStatusCodeHelpers.hpp"
#include "StringBase.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::pair<HttpStatusCode, std::string>
	REASON_PHRASE_STATUS_CODE_PAIRS[] = {
		std::pair<HttpStatusCode, std::string>(CONTINUE, "Continue"),
		std::pair<HttpStatusCode, std::string>(SWITCHING_PROTOCOLS,
                                               "Switching Protocols"),
		std::pair<HttpStatusCode, std::string>(PROCESSING, "Processing"),
		std::pair<HttpStatusCode, std::string>(EARLY_HINTS, "Early Hints"),
		std::pair<HttpStatusCode, std::string>(OK, "OK"),
		std::pair<HttpStatusCode, std::string>(CREATED, "Created"),
		std::pair<HttpStatusCode, std::string>(ACCEPTED, "Accepted"),
		std::pair<HttpStatusCode, std::string>(NON_AUTHORITATIVE_INFORMATION,
                                               "Non-Authoritative Information"),
		std::pair<HttpStatusCode, std::string>(NO_CONTENT, "No Content"),
		std::pair<HttpStatusCode, std::string>(RESET_CONTENT, "Reset Content"),
		std::pair<HttpStatusCode, std::string>(PARTIAL_CONTENT,
                                               "Partial Content"),
		std::pair<HttpStatusCode, std::string>(MULTI_STATUS, "Multi-Status"),
		std::pair<HttpStatusCode, std::string>(ALREADY_REPORTED,
                                               "Already Reported"),
		std::pair<HttpStatusCode, std::string>(IM_USED, "IM Used"),
		std::pair<HttpStatusCode, std::string>(MOVED_PERMANENTLY,
                                               "Moved Permanently"),
		std::pair<HttpStatusCode, std::string>(FOUND, "Found"),
		std::pair<HttpStatusCode, std::string>(SEE_OTHER, "See Other"),
		std::pair<HttpStatusCode, std::string>(NOT_MODIFIED, "Not Modified"),
		std::pair<HttpStatusCode, std::string>(USE_PROXY, "Use Proxy"),
		std::pair<HttpStatusCode, std::string>(SWITCH_PROXY, "Switch Proxy"),
		std::pair<HttpStatusCode, std::string>(TEMPORARY_REDIRECT,
                                               "Temporary Redirect"),
		std::pair<HttpStatusCode, std::string>(PERMANENT_REDIRECT,
                                               "Permanent Redirect"),
		std::pair<HttpStatusCode, std::string>(BAD_REQUEST, "Bad Request"),
		std::pair<HttpStatusCode, std::string>(UNAUTHORIZED, "Unauthorized"),
		std::pair<HttpStatusCode, std::string>(PAYMENT_REQUIRED,
                                               "Payment Required"),
		std::pair<HttpStatusCode, std::string>(FORBIDDEN, "Forbidden"),
		std::pair<HttpStatusCode, std::string>(NOT_FOUND, "Not Found"),
		std::pair<HttpStatusCode, std::string>(METHOD_NOT_ALLOWED,
                                               "Method Not Allowed"),
		std::pair<HttpStatusCode, std::string>(NOT_ACCEPTABLE,
                                               "Not Acceptable"),
		std::pair<HttpStatusCode, std::string>(PROXY_AUTHENTICATION_REQUIRED,
                                               "Proxy Authentication Required"),
		std::pair<HttpStatusCode, std::string>(REQUEST_TIMEOUT,
                                               "Request Timeout"),
		std::pair<HttpStatusCode, std::string>(CONFLICT, "Conflict"),
		std::pair<HttpStatusCode, std::string>(GONE, "Gone"),
		std::pair<HttpStatusCode, std::string>(LENGTH_REQUIRED,
                                               "Length Required"),
		std::pair<HttpStatusCode, std::string>(PRECONDITION_FAILED,
                                               "Precondition Failed"),
		std::pair<HttpStatusCode, std::string>(CONTENT_TOO_LARGE,
                                               "Content Too Large"),
		std::pair<HttpStatusCode, std::string>(URI_TOO_LONG, "URI Too Long"),
		std::pair<HttpStatusCode, std::string>(UNSUPPORTED_MEDIA_TYPE,
                                               "Unsupported Media Type"),
		std::pair<HttpStatusCode, std::string>(RANGE_NOT_SATISFIABLE,
                                               "Range Not Satisfiable"),
		std::pair<HttpStatusCode, std::string>(EXPECTATION_FAILED,
                                               "Expectation Failed"),
		std::pair<HttpStatusCode, std::string>(IM_A_TEAPOT, "I'm a teapot"),
		std::pair<HttpStatusCode, std::string>(MISDIRECTED_REQUEST,
                                               "Misdirected Request"),
		std::pair<HttpStatusCode, std::string>(UNPROCESSABLE_CONTENT,
                                               "Unprocessable Content"),
		std::pair<HttpStatusCode, std::string>(LOCKED, "Locked"),
		std::pair<HttpStatusCode, std::string>(FAILED_DEPENDENCY,
                                               "Failed Dependency"),
		std::pair<HttpStatusCode, std::string>(TOO_EARLY, "Too Early"),
		std::pair<HttpStatusCode, std::string>(UPGRADE_REQUIRED,
                                               "Upgrade Required"),
		std::pair<HttpStatusCode, std::string>(PRECONDITION_REQUIRED,
                                               "Precondition Required"),
		std::pair<HttpStatusCode, std::string>(TOO_MANY_REQUESTS,
                                               "Too Many Requests"),
		std::pair<HttpStatusCode, std::string>(
			REQUEST_HEADER_FIELDS_TOO_LARGE, "Request Header Fields Too Large"),
		std::pair<HttpStatusCode, std::string>(UNAVAILABLE_FOR_LEGAL_REASONS,
                                               "Unavailable For Legal Reasons"),
		std::pair<HttpStatusCode, std::string>(INTERNAL_SERVER_ERROR,
                                               "Internal Server Error"),
		std::pair<HttpStatusCode, std::string>(NOT_IMPLEMENTED,
                                               "Not Implemented"),
		std::pair<HttpStatusCode, std::string>(BAD_GATEWAY, "Bad Gateway"),
		std::pair<HttpStatusCode, std::string>(SERVICE_UNAVAILABLE,
                                               "Service Unavailable"),
		std::pair<HttpStatusCode, std::string>(GATEWAY_TIMEOUT,
                                               "Gateway Timeout"),
		std::pair<HttpStatusCode, std::string>(HTTP_VERSION_NOT_SUPPORTED,
                                               "HTTP Version Not Supported"),
		std::pair<HttpStatusCode, std::string>(VARIANT_ALSO_NEGOTIATES,
                                               "Variant Also Negotiates"),
		std::pair<HttpStatusCode, std::string>(INSUFFICIENT_STORAGE,
                                               "Insufficient Storage"),
		std::pair<HttpStatusCode, std::string>(LOOP_DETECTED, "Loop Detected"),
		std::pair<HttpStatusCode, std::string>(NOT_EXTENDED, "Not Extended"),
		std::pair<HttpStatusCode, std::string>(
			NETWORK_AUTHENTICATION_REQUIRED, "Network Authentication Required"),
};

const std::map<HttpStatusCode, std::string> HttpStatusCodeHelpers::
	REASON_PHRASES_FOR_STATUS_CODES(
		REASON_PHRASE_STATUS_CODE_PAIRS,
		REASON_PHRASE_STATUS_CODE_PAIRS +
			(sizeof(REASON_PHRASE_STATUS_CODE_PAIRS) /
             sizeof(std::pair<HttpStatusCode, std::string>)));

// NOLINTEND(bugprone-throwing-static-initialization)

const std::string &HttpStatusCodeHelpers::getReasonPhraseForStatusCode(
	HttpStatusCode httpStatusCode)
{
	std::map<HttpStatusCode, std::string>::const_iterator iterator =
		REASON_PHRASES_FOR_STATUS_CODES.find(httpStatusCode);
	if (iterator == REASON_PHRASES_FOR_STATUS_CODES.end())
	{
		throw std::out_of_range(StringBase()
		                        << "Unknown HTTP status code \""
		                        << static_cast<int>(httpStatusCode) << "\"");
	}
	return iterator->second;
}

bool HttpStatusCodeHelpers::isInValidRange(HttpStatusCode httpStatusCode)
{
	HttpStatusCodeFamily family = getStatusCodeFamily(httpStatusCode);
	return family == INFORMATIONAL || family == SUCCESS ||
	       family == REDIRECTION || family == CLIENT_ERROR ||
	       family == SERVER_ERROR;
}

bool HttpStatusCodeHelpers::isInformational(HttpStatusCode httpStatusCode)
{
	return getStatusCodeFamily(httpStatusCode) == INFORMATIONAL;
}

bool HttpStatusCodeHelpers::isSuccess(HttpStatusCode httpStatusCode)
{
	return getStatusCodeFamily(httpStatusCode) == SUCCESS;
}

bool HttpStatusCodeHelpers::isRedirection(HttpStatusCode httpStatusCode)
{
	return getStatusCodeFamily(httpStatusCode) == REDIRECTION;
}

bool HttpStatusCodeHelpers::isClientError(HttpStatusCode httpStatusCode)
{
	return getStatusCodeFamily(httpStatusCode) == CLIENT_ERROR;
}

bool HttpStatusCodeHelpers::isServerError(HttpStatusCode httpStatusCode)
{
	return getStatusCodeFamily(httpStatusCode) == SERVER_ERROR;
}

bool HttpStatusCodeHelpers::takesNoValue(HttpStatusCode httpStatusCode)
{
	return httpStatusCode == NO_CONTENT || httpStatusCode == NOT_MODIFIED;
}

HttpStatusCodeFamily HttpStatusCodeHelpers::getStatusCodeFamily(
	HttpStatusCode httpStatusCode)
{
	return static_cast<HttpStatusCodeFamily>(httpStatusCode / HUNDRED *
	                                         HUNDRED);
}
