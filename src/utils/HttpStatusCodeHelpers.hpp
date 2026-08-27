/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCodeHelpers.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 16:55:39 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 13:26:49 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_STATUS_CODE_HELPERS_HPP
#define HTTP_STATUS_CODE_HELPERS_HPP

#include <map>
#include <string>

#include "HttpStatusCode.hpp"
#include "HttpStatusCodeFamily.hpp"

class HttpStatusCodeHelpers
{
public:
	static const std::string &getReasonPhraseForStatusCode(
		HttpStatusCode httpStatusCode);

	static bool isInValidRange(HttpStatusCode httpStatusCode);

	static bool isInformational(HttpStatusCode httpStatusCode);
	static bool isSuccess(HttpStatusCode httpStatusCode);
	static bool isRedirection(HttpStatusCode httpStatusCode);
	static bool isClientError(HttpStatusCode httpStatusCode);
	static bool isServerError(HttpStatusCode httpStatusCode);

	static bool takesNoValue(HttpStatusCode httpStatusCode);

private:
	static const int HUNDRED = 100;

	static const std::map<HttpStatusCode, std::string>
		REASON_PHRASES_FOR_STATUS_CODES;

	static HttpStatusCodeFamily getStatusCodeFamily(
		HttpStatusCode httpStatusCode);
};

#endif
