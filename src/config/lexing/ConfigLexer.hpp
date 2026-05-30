/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 03:19:19 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 23:55:32 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_LEXER_HPP
#define CONFIG_LEXER_HPP

#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

#include "AConfigToken.hpp"

class ConfigLexer
{
public:
	ConfigLexer(void);
	ConfigLexer(const std::string &configFilePath);
	~ConfigLexer(void);

	ConfigLexer &operator>>(AConfigToken **configToken);

	void throwForUnclosedQuoteOrUnterminatedEscape(void) const;

	class ConfigFileNotFoundException: public std::runtime_error
	{
	public:
		ConfigFileNotFoundException(const std::string &configFilePath);
	};

protected:
	ConfigLexer(const ConfigLexer &other);
	ConfigLexer &operator=(const ConfigLexer &other);

private:
	std::ifstream configStream;
	std::string currentConfigLine;
	std::size_t currentConfigLineNumber;
	std::string::iterator currentConfigLineIterator;
	AConfigToken *currentConfigToken;

	enum EscapeMode
	{
		UNESCAPED,
		ESCAPED,
	} escapeMode;

	enum QuoteMode
	{
		UNQUOTED,
		SINGLE_QUOTED,
		DOUBLE_QUOTED,
	} quoteMode;

	std::size_t mostRecentOpeningQuoteLineNumber;
	std::size_t mostRecentOpeningQuoteCharacterNumber;

	typedef bool (ConfigLexer::*t_lexing_func)(void);

	bool delimitCurrentConfigTokenIfConfigStreamExhausted(void);
	bool getNextConfigLineIfCurrentConfigLineExhausted(void);
	bool leaveEscapedSection(void);
	bool leaveQuotedSection(void);
	bool continueQuotedSection(void);
	bool enterEscapedSection(void);
	bool enterQuotedSection(void);
	bool delimitWordIfUnquotedNonWordCharacterEncountered(void);
	bool handleSpecialCharacter(void);
	bool handleWhitespace(void);
	bool handleComment(void);
	bool beginOrContinueWord(void);
};

#endif
