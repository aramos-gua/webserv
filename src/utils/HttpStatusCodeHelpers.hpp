/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCodeHelpers.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 16:55:39 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/23 17:44:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_STATUS_CODE_HELPERS_HPP
#define HTTP_STATUS_CODE_HELPERS_HPP

#include "HttpStatusCode.hpp"
#include "HttpStatusCodeFamily.hpp"

class HttpStatusCodeHelpers
{
public:
	static bool isInValidRange(HttpStatusCode httpStatusCode);

	static bool isInformational(HttpStatusCode httpStatusCode);
	static bool isSuccess(HttpStatusCode httpStatusCode);
	static bool isRedirection(HttpStatusCode httpStatusCode);
	static bool isClientError(HttpStatusCode httpStatusCode);
	static bool isServerError(HttpStatusCode httpStatusCode);

	static bool takesNoValue(HttpStatusCode httpStatusCode);

private:
	static const int HUNDRED = 100;
	static HttpStatusCodeFamily getStatusCodeFamily(
		HttpStatusCode httpStatusCode);
};

#endif
