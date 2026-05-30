/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCodeFamily.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:00:37 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:02:21 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_STATUS_CODE_FAMILY
#define HTTP_STATUS_CODE_FAMILY

enum HttpStatusCodeFamily
{
	INFORMATIONAL = 100,
	SUCCESS = 200,
	REDIRECTION = 300,
	CLIENT_ERROR = 400,
	SERVER_ERROR = 500,
};

#endif
