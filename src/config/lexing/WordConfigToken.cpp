/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WordConfigToken.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:43:02 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 16:56:13 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WordConfigToken.hpp"

WordConfigToken::WordConfigToken(void): AConfigToken(WORD)
{
}

WordConfigToken::WordConfigToken(char content)
	: AConfigToken(WORD), content(std::string(1, content))
{
}

WordConfigToken::WordConfigToken(const std::string &content)
	: AConfigToken(WORD), content(content)
{
}

WordConfigToken::WordConfigToken(const WordConfigToken &other)
	: AConfigToken(other), content(other.content)
{
}

WordConfigToken &WordConfigToken::operator=(const WordConfigToken &other)
{
	if (this == &other)
	{
		return *this;
	}
	content = other.content;
	return *this;
}

WordConfigToken::~WordConfigToken(void)
{
}

const std::string &WordConfigToken::getContent(void) const
{
	return content;
}

void WordConfigToken::extendContentWith(char character)
{
	content += character;
}

void WordConfigToken::extendContentWith(const std::string &string)
{
	content += string;
}
