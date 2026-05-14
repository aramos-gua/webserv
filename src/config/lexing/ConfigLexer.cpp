/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 03:19:23 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 23:56:40 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <stdexcept>

#include "ConfigLexer.hpp"
#include "SpecialCharacterConfigToken.hpp"
#include "StringBase.hpp"
#include "TerminatingConfigToken.hpp"
#include "WordConfigToken.hpp"

ConfigLexer::ConfigLexer(void)
	: configStream(NULL), currentConfigLineNumber(0),
	  currentConfigLineIterator(currentConfigLine.end()),
	  currentConfigToken(NULL), escapeMode(UNESCAPED), quoteMode(UNQUOTED),
	  mostRecentOpeningQuoteLineNumber(0),
	  mostRecentOpeningQuoteCharacterNumber(0)
{
}

ConfigLexer::ConfigLexer(const std::string &configFilePath)
	: configStream(configFilePath), currentConfigLineNumber(0),
	  currentConfigLineIterator(currentConfigLine.end()),
	  currentConfigToken(NULL), escapeMode(UNESCAPED), quoteMode(UNQUOTED),
	  mostRecentOpeningQuoteLineNumber(0),
	  mostRecentOpeningQuoteCharacterNumber(0)
{
	if (!configStream)
	{
		throw std::runtime_error(StringBase()
		                         << "Couldn't open config file \""
		                         << configFilePath << "\" for reading");
	}
}

ConfigLexer::ConfigLexer(const ConfigLexer &other)
	: configStream(NULL), currentConfigLineNumber(0), currentConfigToken(NULL),
	  escapeMode(UNESCAPED), quoteMode(UNQUOTED),
	  mostRecentOpeningQuoteLineNumber(0),
	  mostRecentOpeningQuoteCharacterNumber(0)
{
	(void)other;
	throw std::runtime_error("ConfigLexer copy constructor not implemented");
}

ConfigLexer &ConfigLexer::operator=(const ConfigLexer &other)
{
	if (this == &other)
	{
		return *this;
	}
	throw std::runtime_error(
		"ConfigLexer copy assignment operator not implemented");
}

ConfigLexer::~ConfigLexer(void)
{
}

ConfigLexer &ConfigLexer::operator>>(AConfigToken **configToken)
{
	static const t_lexing_func LEXING_FUNCS[] = {
		&ConfigLexer::delimitCurrentConfigTokenIfConfigStreamExhausted,
		&ConfigLexer::getNextConfigLineIfCurrentConfigLineExhausted,
		&ConfigLexer::leaveEscapedSection,
		&ConfigLexer::leaveQuotedSection,
		&ConfigLexer::continueQuotedSection,
		&ConfigLexer::enterEscapedSection,
		&ConfigLexer::enterQuotedSection,
		&ConfigLexer::delimitWordIfUnquotedNonWordCharacterEncountered,
		&ConfigLexer::handleSpecialCharacter,
		&ConfigLexer::handleWhitespace,
		&ConfigLexer::handleComment,
		&ConfigLexer::beginOrContinueWord,
	};

	while (!currentConfigToken || !currentConfigToken->getIsDelimited())
	{
		for (std::size_t i = 0;
		     i < sizeof(LEXING_FUNCS) / sizeof(t_lexing_func); i++)
		{
			if ((this->*LEXING_FUNCS[i])())
			{
				break;
			}
		}
	}
	*configToken = currentConfigToken;
	currentConfigToken = NULL;
	return *this;
}

void ConfigLexer::throwForUnclosedQuoteOrUnterminatedEscape(void) const
{
	if (quoteMode != UNQUOTED)
	{
		throw std::runtime_error(
			StringBase() << "Opening quote at line "
						 << mostRecentOpeningQuoteLineNumber << ", character "
						 << mostRecentOpeningQuoteCharacterNumber
						 << " of config file was not closed");
	}
	if (escapeMode != UNESCAPED)
	{
		throw std::runtime_error(
			StringBase()
			<< "Unexpected end of file after escape character at line "
			<< currentConfigLineNumber << ", character "
			<< (currentConfigLineIterator - currentConfigLine.begin())
			<< " of config file");
	}
}

bool ConfigLexer::delimitCurrentConfigTokenIfConfigStreamExhausted(void)
{
	if (currentConfigLineIterator != currentConfigLine.end() ||
	    configStream.good())
	{
		return false;
	}
	if (!currentConfigToken)
	{
		currentConfigToken = new TerminatingConfigToken();
	}
	currentConfigToken->delimit();
	return true;
}

bool ConfigLexer::getNextConfigLineIfCurrentConfigLineExhausted(void)
{
	if (currentConfigLineIterator != currentConfigLine.end())
	{
		return false;
	}
	if (escapeMode == UNESCAPED && currentConfigToken)
	{
		if (quoteMode == UNQUOTED)
		{
			currentConfigToken->delimit();
		}
		else
		{
			dynamic_cast<WordConfigToken *>(currentConfigToken)
				->extendContentWith('\n');
		}
	}
	std::getline(configStream, currentConfigLine);
	currentConfigLineNumber++;
	currentConfigLineIterator = currentConfigLine.begin();
	return true;
}

bool ConfigLexer::leaveEscapedSection(void)
{
	if (escapeMode == UNESCAPED)
	{
		return false;
	}
	if (currentConfigLineIterator != currentConfigLine.begin())
	{
		dynamic_cast<WordConfigToken *>(currentConfigToken)
			->extendContentWith(*currentConfigLineIterator);
		++currentConfigLineIterator;
	}
	escapeMode = UNESCAPED;
	return true;
}

bool ConfigLexer::leaveQuotedSection(void)
{
	if ((quoteMode != SINGLE_QUOTED || *currentConfigLineIterator != '\'') &&
	    (quoteMode != DOUBLE_QUOTED || *currentConfigLineIterator != '\"'))
	{
		return false;
	}
	dynamic_cast<WordConfigToken *>(currentConfigToken)
		->extendContentWith(*currentConfigLineIterator);
	++currentConfigLineIterator;
	quoteMode = UNQUOTED;
	return true;
}

bool ConfigLexer::continueQuotedSection(void)
{
	if (quoteMode == UNQUOTED)
	{
		return false;
	}
	std::string::iterator originalIterator = currentConfigLineIterator;
	while (currentConfigLineIterator != currentConfigLine.end() &&
	       *currentConfigLineIterator != '\\' &&
	       (quoteMode != SINGLE_QUOTED || *currentConfigLineIterator != '\'') &&
	       (quoteMode != DOUBLE_QUOTED || *currentConfigLineIterator != '\"'))
	{
		++currentConfigLineIterator;
	}
	if (originalIterator == currentConfigLineIterator)
	{
		return false;
	}
	std::string quotedSectionSubstring(originalIterator,
	                                   currentConfigLineIterator);
	dynamic_cast<WordConfigToken *>(currentConfigToken)
		->extendContentWith(quotedSectionSubstring);
	return true;
}

bool ConfigLexer::enterEscapedSection(void)
{
	if (*currentConfigLineIterator != '\\')
	{
		return false;
	}
	if (currentConfigLineIterator + 1 != currentConfigLine.end())
	{
		if (!currentConfigToken)
		{
			currentConfigToken =
				new WordConfigToken(*currentConfigLineIterator);
		}
		else
		{
			dynamic_cast<WordConfigToken *>(currentConfigToken)
				->extendContentWith(*currentConfigLineIterator);
		}
	}
	escapeMode = ESCAPED;
	++currentConfigLineIterator;
	return true;
}

bool ConfigLexer::enterQuotedSection(void)
{
	if (std::string("\'\"").find(*currentConfigLineIterator) ==
	    std::string::npos)
	{
		return false;
	}
	if (!currentConfigToken)
	{
		currentConfigToken = new WordConfigToken(*currentConfigLineIterator);
	}
	else
	{
		dynamic_cast<WordConfigToken *>(currentConfigToken)
			->extendContentWith(*currentConfigLineIterator);
	}
	switch (*currentConfigLineIterator)
	{
	case '\'':
		quoteMode = SINGLE_QUOTED;
		break;
	case '\"':
		quoteMode = DOUBLE_QUOTED;
		break;
	}
	++currentConfigLineIterator;
	mostRecentOpeningQuoteLineNumber = currentConfigLineNumber;
	mostRecentOpeningQuoteCharacterNumber =
		currentConfigLineIterator - currentConfigLine.begin();
	return true;
}

bool ConfigLexer::delimitWordIfUnquotedNonWordCharacterEncountered(void)
{
	if (!currentConfigToken || quoteMode != UNQUOTED ||
	    (!isspace(*currentConfigLineIterator) &&
	     std::string("#;{}").find(*currentConfigLineIterator) ==
	         std::string::npos))
	{
		return false;
	}
	currentConfigToken->delimit();
	return true;
}

bool ConfigLexer::handleSpecialCharacter(void)
{
	if (std::string(";{}").find(*currentConfigLineIterator) ==
	    std::string::npos)
	{
		return false;
	}
	switch (*currentConfigLineIterator)
	{
	case ';':
		currentConfigToken = new SpecialCharacterConfigToken(
			SpecialCharacterConfigToken::SEMICOLON);
		break;
	case '{':
		currentConfigToken = new SpecialCharacterConfigToken(
			SpecialCharacterConfigToken::LEFT_BRACE);
		break;
	case '}':
		currentConfigToken = new SpecialCharacterConfigToken(
			SpecialCharacterConfigToken::RIGHT_BRACE);
		break;
	}
	currentConfigToken->delimit();
	++currentConfigLineIterator;
	return true;
}

bool ConfigLexer::handleWhitespace(void)
{
	if (!isspace(*currentConfigLineIterator))
	{
		return false;
	}
	while (currentConfigLineIterator != currentConfigLine.end() &&
	       isspace(*currentConfigLineIterator))
	{
		++currentConfigLineIterator;
	}
	return true;
}

bool ConfigLexer::handleComment(void)
{
	if (*currentConfigLineIterator != '#')
	{
		return false;
	}
	while (currentConfigLineIterator != currentConfigLine.end())
	{
		++currentConfigLineIterator;
	}
	return true;
}

bool ConfigLexer::beginOrContinueWord(void)
{
	std::string::iterator originalIterator = currentConfigLineIterator;
	while (currentConfigLineIterator != currentConfigLine.end() &&
	       !isspace(*currentConfigLineIterator) &&
	       std::string("\\\'\"#;{}").find(*currentConfigLineIterator) ==
	           std::string::npos)
	{
		++currentConfigLineIterator;
	}
	std::string substring(originalIterator, currentConfigLineIterator);
	if (!currentConfigToken)
	{
		currentConfigToken = new WordConfigToken(substring);
	}
	else
	{
		dynamic_cast<WordConfigToken *>(currentConfigToken)
			->extendContentWith(substring);
	}
	return true;
}
