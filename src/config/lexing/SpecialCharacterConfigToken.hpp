/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpecialCharacterConfigToken.hpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:36:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 16:56:02 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPECIAL_CHARACTER_CONFIG_TOKEN_HPP
#define SPECIAL_CHARACTER_CONFIG_TOKEN_HPP

#include <cstddef>

#include "AConfigToken.hpp"

class SpecialCharacterConfigToken: public AConfigToken
{
public:
	enum SpecialCharacter
	{
		SEMICOLON,
		LEFT_BRACE,
		RIGHT_BRACE,
	};

	SpecialCharacterConfigToken(void);
	SpecialCharacterConfigToken(SpecialCharacter content,
	                            std::size_t lineNumber);
	SpecialCharacterConfigToken(const SpecialCharacterConfigToken &other);
	SpecialCharacterConfigToken &operator=(
		const SpecialCharacterConfigToken &other);
	~SpecialCharacterConfigToken(void);

	SpecialCharacter getContent(void) const;

private:
	SpecialCharacter content;
};

#endif
