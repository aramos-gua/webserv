/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageValue.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 03:54:39 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 08:29:58 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_PAGE_VALUE_HPP
#define ERROR_PAGE_VALUE_HPP

#include <string>

#include "HttpStatusCode.hpp"

class ErrorPageValue
{
public:
	ErrorPageValue(void);
	ErrorPageValue(HttpStatusCode httpStatusCodeToReturn,
	               const std::string &uri);
	ErrorPageValue(const ErrorPageValue &other);
	ErrorPageValue &operator=(const ErrorPageValue &other);
	~ErrorPageValue(void);

	HttpStatusCode getHttpStatusCodeToReturn(void) const;
	const std::string &getUri(void) const;

private:
	HttpStatusCode httpStatusCodeToReturn;
	std::string uri;
};

#endif
