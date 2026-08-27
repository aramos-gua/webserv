/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBodyReader.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 20:30:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 20:30:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <vector>

#include "ResponseBodyReader.hpp"

ssize_t ResponseBodyReader::appendUpTo(int fileDescriptor,
                                       std::size_t remaining,
                                       std::size_t readSize,
                                       std::string &buffer)
{
	if (remaining == 0 || readSize == 0)
	{
		return 0;
	}
	// Never ask for more than is still owed, so that a body sharing a
	// descriptor with something after it cannot read into what follows.
	std::size_t wanted = readSize < remaining ? readSize : remaining;
	// std::vector::data() is C++11, so the address of the first element is
	// the only way to reach the buffer here.
	// NOLINTBEGIN(readability-container-data-pointer)
	std::vector<char> readBuffer(wanted);
	ssize_t bytesRead = read(fileDescriptor, &readBuffer[0], wanted);

	if (bytesRead <= 0)
	{
		return bytesRead;
	}
	buffer.append(&readBuffer[0], static_cast<std::size_t>(bytesRead));
	// NOLINTEND(readability-container-data-pointer)
	return bytesRead;
}
