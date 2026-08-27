/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:36:38 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 16:27:17 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_METHOD_HPP
#define HTTP_METHOD_HPP

enum HttpMethod
{
	NO_METHOD = 0,

	GET,
	HEAD,
	POST,
	PUT,
	PATCH,
	DELETE,
	CONNECT,
	OPTIONS,
	TRACE
};

#endif
