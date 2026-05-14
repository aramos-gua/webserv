/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WordConfigToken.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:29:51 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 16:56:15 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_CONFIG_TOKEN_HPP
#define WORD_CONFIG_TOKEN_HPP

#include <string>

#include "AConfigToken.hpp"

class WordConfigToken: public AConfigToken
{
public:
	WordConfigToken(void);
	WordConfigToken(char content);
	WordConfigToken(const std::string &content);
	WordConfigToken(const WordConfigToken &other);
	WordConfigToken &operator=(const WordConfigToken &other);
	~WordConfigToken(void);

	const std::string &getContent(void) const;

	void extendContentWith(char character);
	void extendContentWith(const std::string &string);

private:
	std::string content;
};

#endif
