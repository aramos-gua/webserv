/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TransferEncoding.hpp                                   :+:      :+:    :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:36:38 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 16:47:53 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRANSFER_ENCODING_HPP
#define TRANSFER_ENCODING_HPP

enum TransferEncoding
{
	NO_TRANSFER_ENCODING = 0,

	CHUNKED,
	COMPRESS,
	DEFLATE,
	GZIP
};

#endif
