/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBodyReader.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:30:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 20:30:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_BODY_READER_HPP
#define RESPONSE_BODY_READER_HPP

#include <cstddef>
#include <string>
#include <sys/types.h>

// Moves a response body that lives outside the response itself — a file on
// disk, or a spooled copy of a CGI script's output — towards a socket, a
// bounded piece at a time.
//
// Every function is static and the file descriptor is only ever borrowed:
// nothing here opens, closes or remembers it. That is deliberate. The response
// is a copyable value with no move constructor to lean on, so an owned
// descriptor inside it would leave a copy having to choose between duplicating
// the descriptor and closing it twice — and closing a descriptor twice, once
// its number has been handed out again, breaks whichever connection now holds
// it rather than the one that made the mistake. Ownership stays with whoever
// outlives the send.
class ResponseBodyReader
{
public:
	static const std::size_t DEFAULT_READ_SIZE = 64UL * 1024UL;

	// Appends at most readSize bytes, and never more than remaining, from
	// fileDescriptor to buffer. Returns how many were appended, zero at end of
	// input, or -1 if the read failed. A short read is not an error and not an
	// end: callers keep going until remaining reaches zero.
	static ssize_t appendUpTo(int fileDescriptor, std::size_t remaining,
	                          std::size_t readSize, std::string &buffer);
};

#endif
