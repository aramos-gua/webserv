/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TerminatingConfigToken.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:39:19 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 16:56:06 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TerminatingConfigToken.hpp"

TerminatingConfigToken::TerminatingConfigToken(void)
	: AConfigToken(TERMINATING, 0)
{
}

TerminatingConfigToken::TerminatingConfigToken(std::size_t lineNumber)
	: AConfigToken(TERMINATING, lineNumber)
{
}

TerminatingConfigToken::TerminatingConfigToken(
	const TerminatingConfigToken &other)
	: AConfigToken(other)
{
}

TerminatingConfigToken &TerminatingConfigToken::operator=(
	const TerminatingConfigToken &other)
{
	(void)other;
	return *this;
}

TerminatingConfigToken::~TerminatingConfigToken(void)
{
}
