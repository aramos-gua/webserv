/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfigToken.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:18:38 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 17:05:46 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>

#include "AConfigToken.hpp"
#include "SpecialCharacterConfigToken.hpp"
#include "StringBase.hpp"
#include "WordConfigToken.hpp"

AConfigToken::AConfigToken(void)
	: type(TERMINATING), isDelimited(false), lineNumber(0)
{
}

AConfigToken::AConfigToken(Type type, std::size_t lineNumber)
	: type(type), isDelimited(false), lineNumber(lineNumber)
{
}

AConfigToken::AConfigToken(const AConfigToken &other)
	: type(other.type), isDelimited(other.isDelimited),
	  lineNumber(other.lineNumber)
{
}

AConfigToken &AConfigToken::operator=(const AConfigToken &other)
{
	if (this == &other)
	{
		return *this;
	}
	if (type != other.type)
	{
		throw std::logic_error(
			StringBase() << "Cannot copy-assign config token of type "
						 << other.type << " to config token of type " << type);
	}
	isDelimited = other.isDelimited;
	lineNumber = other.lineNumber;
	return *this;
}

AConfigToken::~AConfigToken(void)
{
}

AConfigToken::Type AConfigToken::getType(void) const
{
	return type;
}

bool AConfigToken::getIsDelimited(void) const
{
	return isDelimited;
}

std::size_t AConfigToken::getLineNumber(void) const
{
	return lineNumber;
}

void AConfigToken::delimit(void)
{
	isDelimited = true;
}

std::ostream &operator<<(std::ostream &stream, const AConfigToken &configToken)
{
	switch (configToken.getType())
	{
	case AConfigToken::SPECIAL_CHARACTER:
		stream << "SpecialCharacterConfigToken ";
		switch (dynamic_cast<const SpecialCharacterConfigToken &>(configToken)
		            .getContent())
		{
		case SpecialCharacterConfigToken::SEMICOLON:
			stream << ';';
			break;
		case SpecialCharacterConfigToken::LEFT_BRACE:
			stream << '{';
			break;
		case SpecialCharacterConfigToken::RIGHT_BRACE:
			stream << '}';
			break;
		}
		break;
	case AConfigToken::TERMINATING:
		stream << "TerminatingConfigToken";
		break;
	case AConfigToken::WORD:
		stream
			<< "WordConfigToken             "
			<< dynamic_cast<const WordConfigToken &>(configToken).getContent();
		break;
	}
	return stream;
}
