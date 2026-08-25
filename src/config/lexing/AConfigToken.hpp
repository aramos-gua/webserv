/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfigToken.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:17:48 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 16:55:46 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_CONFIG_TOKEN_HPP
#define A_CONFIG_TOKEN_HPP

#include <cstddef>
#include <ostream>

class AConfigToken
{
public:
	enum Type
	{
		TERMINATING,
		WORD,
		SPECIAL_CHARACTER,
	};

	AConfigToken(void);
	AConfigToken(Type type, std::size_t lineNumber);
	AConfigToken(const AConfigToken &other);
	AConfigToken &operator=(const AConfigToken &other);
	virtual ~AConfigToken(void) = 0;

	Type getType(void) const;
	bool getIsDelimited(void) const;
	std::size_t getLineNumber(void) const;

	void delimit(void);

private:
	Type type;
	bool isDelimited;
	std::size_t lineNumber;
};

std::ostream &operator<<(std::ostream &stream, const AConfigToken &configToken);

#endif
