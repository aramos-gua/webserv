/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersion.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:36:38 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 16:47:53 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_VERSION_HPP
#define HTTP_VERSION_HPP

enum HttpVersion
{
	NO_VERSION = 0,

	HTTP_1_0,
	HTTP_1_1,
	HTTP_2,
	HTTP_3
};

#endif
