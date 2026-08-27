/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TransferEncodingHelpers.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:36:38 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 17:29:45 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRANSFER_ENCODING_HELPERS_HPP
#define TRANSFER_ENCODING_HELPERS_HPP

#include <map>
#include <set>
#include <string>

#include "TransferEncoding.hpp"

class TransferEncodingHelpers
{
public:
	static TransferEncoding getTransferEncodingForString(
		const std::string &transferEncodingString);
	static std::string getStringForTransferEncoding(
		TransferEncoding transferEncoding);

	static bool getWhetherTransferEncodingIsSupported(
		TransferEncoding transferEncoding);

private:
	static const std::map<std::string, TransferEncoding>
		TRANSFER_ENCODINGS_FOR_STRINGS;
	static const std::map<TransferEncoding, std::string>
		STRINGS_FOR_TRANSFER_ENCODINGS;
	static const std::set<TransferEncoding> SUPPORTED_TRANSFER_ENCODINGS;
};

#endif
