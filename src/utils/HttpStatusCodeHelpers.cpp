/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCodeHelpers.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 16:55:42 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/03 09:29:43 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpStatusCodeHelpers.hpp"

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

HttpStatusCodeFamily HttpStatusCodeHelpers::getStatusCodeFamily(
	HttpStatusCode httpStatusCode)
{
	return static_cast<HttpStatusCodeFamily>(httpStatusCode / HUNDRED *
	                                         HUNDRED);
}
