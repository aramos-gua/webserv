/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TransferEncodingHelpers.cpp                                   :+:      :+:
 * :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:36:38 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 16:47:53 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <utility>

#include "StringBase.hpp"
#include "StringHelpers.hpp"
#include "TransferEncodingHelpers.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)

static const std::pair<std::string, TransferEncoding>
	TRANSFER_ENCODING_STRING_PAIRS[] = {
		std::pair<std::string, TransferEncoding>("chunked", CHUNKED),
		std::pair<std::string, TransferEncoding>("compress", COMPRESS),
		std::pair<std::string, TransferEncoding>("deflate", DEFLATE),
		std::pair<std::string, TransferEncoding>("gzip", GZIP),
};

static const std::pair<TransferEncoding, std::string>
	TRANSFER_ENCODING_ENUM_PAIRS[] = {
		std::pair<TransferEncoding, std::string>(CHUNKED, "chunked"),
		std::pair<TransferEncoding, std::string>(COMPRESS, "compress"),
		std::pair<TransferEncoding, std::string>(DEFLATE, "deflate"),
		std::pair<TransferEncoding, std::string>(GZIP, "gzip"),
};

static const TransferEncoding SUPPORTED_TRANSFER_ENCODING_ARRAY[] = {
	CHUNKED,
};

const std::map<std::string, TransferEncoding> TransferEncodingHelpers::
	TRANSFER_ENCODINGS_FOR_STRINGS(
		TRANSFER_ENCODING_STRING_PAIRS,
		TRANSFER_ENCODING_STRING_PAIRS +
			(sizeof(TRANSFER_ENCODING_STRING_PAIRS) /
             sizeof(std::pair<std::string, TransferEncoding>)));

const std::map<TransferEncoding, std::string> TransferEncodingHelpers::
	STRINGS_FOR_TRANSFER_ENCODINGS(
		TRANSFER_ENCODING_ENUM_PAIRS,
		TRANSFER_ENCODING_ENUM_PAIRS +
			(sizeof(TRANSFER_ENCODING_ENUM_PAIRS) /
             sizeof(std::pair<TransferEncoding, std::string>)));

const std::set<TransferEncoding> TransferEncodingHelpers::
	SUPPORTED_TRANSFER_ENCODINGS(
		SUPPORTED_TRANSFER_ENCODING_ARRAY,
		SUPPORTED_TRANSFER_ENCODING_ARRAY +
			(sizeof(SUPPORTED_TRANSFER_ENCODING_ARRAY) /
             sizeof(TransferEncoding)));

// NOLINTEND(bugprone-throwing-static-initialization)

TransferEncoding TransferEncodingHelpers::getTransferEncodingForString(
	const std::string &transferEncodingString)
{
	std::map<std::string, TransferEncoding>::const_iterator iterator =
		TRANSFER_ENCODINGS_FOR_STRINGS.find(
			StringHelpers::toLowercase(transferEncodingString));
	if (iterator == TRANSFER_ENCODINGS_FOR_STRINGS.end())
	{
		throw std::out_of_range(StringBase() << "Unknown transfer coding \""
		                                     << transferEncodingString << "\"");
	}
	return iterator->second;
}

std::string TransferEncodingHelpers::getStringForTransferEncoding(
	TransferEncoding transferEncoding)
{
	std::map<TransferEncoding, std::string>::const_iterator iterator =
		STRINGS_FOR_TRANSFER_ENCODINGS.find(transferEncoding);
	if (iterator == STRINGS_FOR_TRANSFER_ENCODINGS.end())
	{
		throw std::out_of_range("Unknown transfer coding");
	}
	return iterator->second;
}

bool TransferEncodingHelpers::getWhetherTransferEncodingIsSupported(
	TransferEncoding transferEncoding)
{
	return SUPPORTED_TRANSFER_ENCODINGS.find(transferEncoding) !=
	       SUPPORTED_TRANSFER_ENCODINGS.end();
}
