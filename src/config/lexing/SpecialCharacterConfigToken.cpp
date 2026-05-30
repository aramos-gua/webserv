/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpecialCharacterConfigToken.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:45:44 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 16:55:58 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SpecialCharacterConfigToken.hpp"

SpecialCharacterConfigToken::SpecialCharacterConfigToken(void)
	: AConfigToken(SPECIAL_CHARACTER, 0), content()
{
}

SpecialCharacterConfigToken::SpecialCharacterConfigToken(
	SpecialCharacter content, std::size_t lineNumber)
	: AConfigToken(SPECIAL_CHARACTER, lineNumber), content(content)
{
}

SpecialCharacterConfigToken::SpecialCharacterConfigToken(
	const SpecialCharacterConfigToken &other)
	: AConfigToken(other), content(other.content)
{
}

SpecialCharacterConfigToken &SpecialCharacterConfigToken::operator=(
	const SpecialCharacterConfigToken &other)
{
	if (this == &other)
	{
		return *this;
	}
	content = other.content;
	return *this;
}

SpecialCharacterConfigToken::~SpecialCharacterConfigToken(void)
{
}

SpecialCharacterConfigToken::SpecialCharacter SpecialCharacterConfigToken::
	getContent(void) const
{
	return content;
}
