/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpFieldHelpers.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 21:40:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 21:22:42 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_FIELD_HELPERS_HPP
#define HTTP_FIELD_HELPERS_HPP

#include <string>

class HttpFieldHelpers
{
public:
	static bool getWhetherFieldNameIsValid(const std::string &fieldName);
};

#endif
