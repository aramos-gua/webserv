/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 03:19:25 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/25 07:26:36 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <climits>
#include <cstdlib>
#include <set>
#include <sstream>
#include <stdexcept>

#include "AConfig.hpp"
#include "AConfigToken.hpp"
#include "AccessRule.hpp"
#include "ConfigParser.hpp"
#include "ConfigTypeHelpers.hpp"
#include "ErrorPageValue.hpp"
#include "EventsConfig.hpp"
#include "HttpConfig.hpp"
#include "HttpMethodHelpers.hpp"
#include "HttpStatusCodeHelpers.hpp"
#include "LimitExceptConfig.hpp"
#include "LimitExceptValue.hpp"
#include "LocationConfig.hpp"
#include "ReturnResponseValue.hpp"
#include "ServerConfig.hpp"
#include "SpecialCharacterConfigToken.hpp"
#include "StringBase.hpp"
#include "StringHelpers.hpp"
#include "TryFilesValue.hpp"
#include "WordConfigToken.hpp"

ConfigParser::ConfigParser(void)
	: currentToken(NULL), currentDirectiveLineNumber(0), activeIncludes(NULL),
	  ownsActiveIncludes(true)
{
	throw std::runtime_error(
		"ConfigParser default constructor not implemented");
}

ConfigParser::ConfigParser(const std::string &configFilePath, AConfig &config)
	: configLexer(configFilePath), currentFilePath(configFilePath),
	  currentToken(NULL), currentDirectiveLineNumber(0),
	  activeIncludes(new std::set<std::string>()), ownsActiveIncludes(true)
{
	parse(config);
}

ConfigParser::ConfigParser(const std::string &configFilePath, AConfig &config,
                           std::set<std::string> *activeIncludes)
	: configLexer(configFilePath), currentFilePath(configFilePath),
	  currentToken(NULL), currentDirectiveLineNumber(0),
	  activeIncludes(activeIncludes), ownsActiveIncludes(false)
{
	parse(config);
}

ConfigParser::ConfigParser(const ConfigParser &other)
	: currentToken(NULL), currentDirectiveLineNumber(0), activeIncludes(NULL),
	  ownsActiveIncludes(true)
{
	(void)other;
	throw std::runtime_error("ConfigParser copy constructor not implemented");
}

ConfigParser &ConfigParser::operator=(const ConfigParser &other)
{
	if (this == &other)
	{
		return *this;
	}
	throw std::runtime_error(
		"ConfigParser copy assignment operator not implemented");
}

ConfigParser::~ConfigParser(void)
{
	delete currentToken;
	activeIncludes->erase(activeIncludeIterator);
	if (ownsActiveIncludes)
	{
		delete activeIncludes;
	}
}

std::string ConfigParser::getAbsolutePath(const std::string &path)
{
	char absolutePathBuffer[PATH_MAX];
	if (realpath(path.c_str(), absolutePathBuffer))
	{
		return std::string(absolutePathBuffer);
	}
	return path;
}

std::map<std::string, ConfigParser::t_handler> ConfigParser::createHandlers(
	void)
{
	std::map<std::string, t_handler> handlers;
	handlers["access_log"] = &ConfigParser::handleAccessLog;
	handlers["alias"] = &ConfigParser::handleAlias;
	handlers["allow"] = &ConfigParser::handleAllow;
	handlers["autoindex"] = &ConfigParser::handleAutoindex;
	handlers["client_max_body_size"] = &ConfigParser::handleClientMaxBodySize;
	handlers["daemon"] = &ConfigParser::handleDaemon;
	handlers["default_type"] = &ConfigParser::handleDefaultType;
	handlers["deny"] = &ConfigParser::handleDeny;
	handlers["error_log"] = &ConfigParser::handleErrorLog;
	handlers["error_page"] = &ConfigParser::handleErrorPage;
	handlers["events"] = &ConfigParser::handleEvents;
	handlers["http"] = &ConfigParser::handleHttp;
	handlers["include"] = &ConfigParser::handleInclude;
	handlers["index"] = &ConfigParser::handleIndex;
	handlers["internal"] = &ConfigParser::handleInternal;
	handlers["limit_except"] = &ConfigParser::handleLimitExcept;
	handlers["listen"] = &ConfigParser::handleListen;
	handlers["location"] = &ConfigParser::handleLocation;
	handlers["return"] = &ConfigParser::handleReturn;
	handlers["root"] = &ConfigParser::handleRoot;
	handlers["server"] = &ConfigParser::handleServer;
	handlers["server_name"] = &ConfigParser::handleServerName;
	handlers["try_files"] = &ConfigParser::handleTryFiles;
	handlers["types"] = &ConfigParser::handleTypes;
	handlers["user"] = &ConfigParser::handleUser;
	handlers["worker_connections"] = &ConfigParser::handleWorkerConnections;
	handlers["worker_processes"] = &ConfigParser::handleWorkerProcesses;
	return handlers;
}

std::string ConfigParser::getLocation(std::size_t lineNumber) const
{
	return StringBase() << "\"" << currentFilePath << "\", line " << lineNumber;
}

void ConfigParser::advance(void)
{
	delete currentToken;
	currentToken = NULL;
	configLexer >> &currentToken;
}

std::string ConfigParser::expectWord(void)
{
	if (currentToken->getType() != AConfigToken::WORD)
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(currentToken->getLineNumber())
		                         << ": Expected value for \""
		                         << currentDirective << "\"");
	}
	std::string word =
		dynamic_cast<WordConfigToken *>(currentToken)->getContent();
	advance();
	return word;
}

void ConfigParser::expectSemicolon(void)
{
	if (currentToken->getType() != AConfigToken::SPECIAL_CHARACTER ||
	    dynamic_cast<SpecialCharacterConfigToken *>(currentToken)
	            ->getContent() != SpecialCharacterConfigToken::SEMICOLON)
	{
		if (currentToken->getType() == AConfigToken::WORD)
		{
			throw std::runtime_error(
				StringBase()
				<< getLocation(currentToken->getLineNumber())
				<< ": Too many arguments for \"" << currentDirective << "\"");
		}
		throw std::runtime_error(StringBase()
		                         << getLocation(currentToken->getLineNumber())
		                         << ": Expected ';' after \""
		                         << currentDirective << "\"");
	}
	advance();
}

void ConfigParser::expectLeftBrace(void)
{
	if (currentToken->getType() != AConfigToken::SPECIAL_CHARACTER ||
	    dynamic_cast<SpecialCharacterConfigToken *>(currentToken)
	            ->getContent() != SpecialCharacterConfigToken::LEFT_BRACE)
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(currentToken->getLineNumber())
		                         << ": Expected '{' for \"" << currentDirective
		                         << "\"");
	}
	advance();
}

void ConfigParser::expectRightBrace(void)
{
	if (currentToken->getType() != AConfigToken::SPECIAL_CHARACTER ||
	    dynamic_cast<SpecialCharacterConfigToken *>(currentToken)
	            ->getContent() != SpecialCharacterConfigToken::RIGHT_BRACE)
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(currentToken->getLineNumber())
		                         << ": Expected '}' for \"" << currentDirective
		                         << "\"");
	}
	advance();
}

bool ConfigParser::expectBool(void)
{
	std::size_t lineNumber = currentToken->getLineNumber();
	std::string value = expectWord();
	if (value == "on")
	{
		return true;
	}
	if (value == "off")
	{
		return false;
	}
	throw std::runtime_error(StringBase()
	                         << getLocation(lineNumber) << ": Invalid value \""
	                         << value << "\" for \"" << currentDirective
	                         << "\": expected 'on' or 'off'");
}

std::size_t ConfigParser::expectSize(void)
{
	std::size_t lineNumber = currentToken->getLineNumber();
	std::string value = expectWord();
	return parseSize(value, lineNumber);
}

std::size_t ConfigParser::expectSizeWithUnits(void)
{
	std::size_t lineNumber = currentToken->getLineNumber();
	std::string value = expectWord();
	std::size_t multiplier = 1;
	std::string digits = value;
	if (!value.empty())
	{
		char last = value[value.size() - 1];
		if (last == 'k' || last == 'K')
		{
			multiplier = BYTES_PER_KILOBYTE;
			digits = value.substr(0, value.size() - 1);
		}
		else if (last == 'm' || last == 'M')
		{
			multiplier = BYTES_PER_KILOBYTE * BYTES_PER_KILOBYTE;
			digits = value.substr(0, value.size() - 1);
		}
	}
	return parseSize(digits, lineNumber) * multiplier;
}

std::vector<ConfigParser::t_word_line_number_pair> ConfigParser::
	expectOneOrMoreWords(void)
{
	std::vector<t_word_line_number_pair> wordLineNumberPairs;
	std::size_t firstWordLineNumber = currentToken->getLineNumber();
	wordLineNumberPairs.push_back(
		std::make_pair(expectWord(), firstWordLineNumber));
	while (currentToken->getType() == AConfigToken::WORD)
	{
		std::size_t lineNumber = currentToken->getLineNumber();
		std::string word =
			dynamic_cast<WordConfigToken *>(currentToken)->getContent();
		advance();
		wordLineNumberPairs.push_back(std::make_pair(word, lineNumber));
	}
	return wordLineNumberPairs;
}

void ConfigParser::parse(AConfig &config)
{
	activeIncludeIterator =
		activeIncludes->insert(getAbsolutePath(currentFilePath)).first;
	advance();
	parseDirectives(config);
	if (currentToken->getType() == AConfigToken::SPECIAL_CHARACTER &&
	    dynamic_cast<SpecialCharacterConfigToken *>(currentToken)
	            ->getContent() == SpecialCharacterConfigToken::RIGHT_BRACE)
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(currentToken->getLineNumber())
		                         << ": Unexpected '}'");
	}
	try
	{
		configLexer.throwForUnclosedQuoteOrUnterminatedEscape();
	}
	catch (const std::runtime_error &e)
	{
		throw std::runtime_error(StringBase() << "\"" << currentFilePath
		                                      << "\": " << e.what());
	}
}

void ConfigParser::parseDirectives(AConfig &config)
{
	static const std::map<std::string, t_handler> HANDLERS = createHandlers();

	std::string savedDirective = currentDirective;
	std::size_t savedDirectiveLineNumber = currentDirectiveLineNumber;
	while (true)
	{
		if (currentToken->getType() == AConfigToken::TERMINATING ||
		    (currentToken->getType() == AConfigToken::SPECIAL_CHARACTER &&
		     dynamic_cast<SpecialCharacterConfigToken *>(currentToken)
		             ->getContent() ==
		         SpecialCharacterConfigToken::RIGHT_BRACE))
		{
			break;
		}
		if (currentToken->getType() == AConfigToken::SPECIAL_CHARACTER)
		{
			SpecialCharacterConfigToken::SpecialCharacter specialCharacter =
				dynamic_cast<SpecialCharacterConfigToken *>(currentToken)
					->getContent();
			const char *symbol =
				specialCharacter == SpecialCharacterConfigToken::SEMICOLON ? ";"
				: specialCharacter == SpecialCharacterConfigToken::LEFT_BRACE
					? "{"
					: "}";
			throw std::runtime_error(
				StringBase() << getLocation(currentToken->getLineNumber())
							 << ": Unexpected '" << symbol << "'");
		}
		std::string name =
			dynamic_cast<WordConfigToken *>(currentToken)->getContent();
		std::size_t lineNumber = currentToken->getLineNumber();
		advance();
		currentDirective = name;
		currentDirectiveLineNumber = lineNumber;
		try
		{
			t_handler handler = HANDLERS.at(name);
			(this->*handler)(config);
		}
		catch (const std::out_of_range &)
		{
			throw std::runtime_error(StringBase()
			                         << getLocation(currentDirectiveLineNumber)
			                         << ": Unknown directive \""
			                         << currentDirective << "\"");
		}
		catch (const AConfig::DirectiveAlreadySetException &)
		{
			throw std::runtime_error(
				StringBase()
				<< getLocation(currentDirectiveLineNumber)
				<< ": Duplicate directive \"" << currentDirective << "\" in \""
				<< ConfigTypeHelpers::getStringForConfigType(
					   config.getConfigType())
				<< "\" context");
		}
		catch (const AConfig::DirectiveNotSupportedForConfigTypeException &)
		{
			throw std::runtime_error(
				StringBase()
				<< getLocation(currentDirectiveLineNumber) << ": Directive \""
				<< currentDirective << "\" not allowed in \""
				<< ConfigTypeHelpers::getStringForConfigType(
					   config.getConfigType())
				<< "\" context");
		}
	}
	currentDirective = savedDirective;
	currentDirectiveLineNumber = savedDirectiveLineNumber;
}

std::size_t ConfigParser::parseSize(const std::string &value,
                                    std::size_t lineNumber)
{
	if (!StringHelpers::isAllDigits(value))
	{
		throw std::runtime_error(
			StringBase() << getLocation(lineNumber) << ": Invalid number \""
						 << value << "\" for \"" << currentDirective << "\"");
	}
	std::istringstream stream(value);
	std::size_t result;
	stream >> result;
	if (stream.fail())
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(lineNumber) << ": Number \""
		                         << value << "\" out of range for \""
		                         << currentDirective << "\"");
	}
	return result;
}

HttpStatusCode ConfigParser::parseHttpStatusCode(const std::string &value,
                                                 std::size_t lineNumber)
{
	HttpStatusCode statusCode =
		static_cast<HttpStatusCode>(parseSize(value, lineNumber));
	if (!HttpStatusCodeHelpers::isInValidRange(statusCode))
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(lineNumber)
		                         << ": Invalid status code \"" << value
		                         << "\" for \"" << currentDirective << "\"");
	}
	return statusCode;
}

void ConfigParser::handleAccessLog(AConfig &config)
{
	std::string path = expectWord();
	expectSemicolon();
	config.setAccessLogFilePath(path);
}

void ConfigParser::handleAlias(AConfig &config)
{
	std::string path = expectWord();
	expectSemicolon();
	config.setAlias(path);
}

void ConfigParser::handleAllow(AConfig &config)
{
	std::string address = expectWord();
	expectSemicolon();
	try
	{
		config.setAccessRuleForIpAddressAndMask(address, ALLOW);
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(currentDirectiveLineNumber)
		                         << ": " << e.what());
	}
}

void ConfigParser::handleAutoindex(AConfig &config)
{
	bool value = expectBool();
	expectSemicolon();
	config.setWhetherShouldUseAutoindex(value);
}

void ConfigParser::handleClientMaxBodySize(AConfig &config)
{
	std::size_t size = expectSizeWithUnits();
	expectSemicolon();
	config.setClientMaxBodySize(size);
}

void ConfigParser::handleDaemon(AConfig &config)
{
	bool value = expectBool();
	expectSemicolon();
	config.setWhetherShouldRunAsDaemon(value);
}

void ConfigParser::handleDefaultType(AConfig &config)
{
	std::string mimeType = expectWord();
	expectSemicolon();
	config.setDefaultMimeType(mimeType);
}

void ConfigParser::handleDeny(AConfig &config)
{
	std::string address = expectWord();
	expectSemicolon();
	try
	{
		config.setAccessRuleForIpAddressAndMask(address, DENY);
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(currentDirectiveLineNumber)
		                         << ": " << e.what());
	}
}

void ConfigParser::handleErrorLog(AConfig &config)
{
	std::string path = expectWord();
	expectSemicolon();
	config.setErrorLogFilePath(path);
}

void ConfigParser::handleErrorPage(AConfig &config)
{
	std::vector<t_word_line_number_pair> wordLineNumberPairs =
		expectOneOrMoreWords();
	if (wordLineNumberPairs.size() < 2)
	{
		throw std::runtime_error(
			StringBase() << getLocation(currentDirectiveLineNumber) << ": \""
						 << currentDirective
						 << "\" requires at least a status code and a URI");
	}
	expectSemicolon();
	const std::string &uri = wordLineNumberPairs.back().first;
	bool hasResponseCodeModifier =
		!wordLineNumberPairs[wordLineNumberPairs.size() - 2].first.empty() &&
		wordLineNumberPairs[wordLineNumberPairs.size() - 2].first[0] == '=';
	HttpStatusCode responseCode = NONE;
	std::size_t statusCodeCount;
	if (hasResponseCodeModifier)
	{
		const t_word_line_number_pair &responseCodeModifierAndLineNumber =
			wordLineNumberPairs[wordLineNumberPairs.size() - 2];
		std::string modifierValue =
			responseCodeModifierAndLineNumber.first.substr(1);
		if (!modifierValue.empty())
		{
			responseCode = parseHttpStatusCode(
				modifierValue, responseCodeModifierAndLineNumber.second);
		}
		statusCodeCount = wordLineNumberPairs.size() - 2;
		if (statusCodeCount < 1)
		{
			throw std::runtime_error(StringBase()
			                         << getLocation(currentDirectiveLineNumber)
			                         << ": \"" << currentDirective
			                         << "\" requires at least one status code");
		}
	}
	else
	{
		statusCodeCount = wordLineNumberPairs.size() - 1;
	}
	for (std::size_t i = 0; i < statusCodeCount; ++i)
	{
		HttpStatusCode statusCode = parseHttpStatusCode(
			wordLineNumberPairs[i].first, wordLineNumberPairs[i].second);
		HttpStatusCode returnCode =
			hasResponseCodeModifier ? responseCode : statusCode;
		config.setErrorPageForHttpStatusCode(statusCode,
		                                     ErrorPageValue(returnCode, uri));
	}
}

void ConfigParser::handleEvents(AConfig &config)
{
	expectLeftBrace();
	EventsConfig eventsConfig;
	parseDirectives(eventsConfig);
	expectRightBrace();
	config.setEventsConfig(eventsConfig);
}

void ConfigParser::handleHttp(AConfig &config)
{
	expectLeftBrace();
	HttpConfig httpConfig;
	parseDirectives(httpConfig);
	expectRightBrace();
	config.setHttpConfig(httpConfig);
}

void ConfigParser::handleInclude(AConfig &config)
{
	std::string path = expectWord();
	expectSemicolon();
	std::string resolvedPath = path;
	if (path.empty() || path[0] != '/')
	{
		std::size_t lastSlash = currentFilePath.rfind('/');
		if (lastSlash != std::string::npos)
		{
			resolvedPath = currentFilePath.substr(0, lastSlash + 1) + path;
		}
	}
	if (activeIncludes->find(getAbsolutePath(resolvedPath)) !=
	    activeIncludes->end())
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(currentDirectiveLineNumber)
		                         << ": Circular include detected for \"" << path
		                         << "\"");
	}
	try
	{
		ConfigParser(resolvedPath, config, activeIncludes);
	}
	catch (const ConfigLexer::ConfigFileNotFoundException &)
	{
		throw std::runtime_error(
			StringBase()
			<< getLocation(currentDirectiveLineNumber) << ": "
			<< ConfigLexer::ConfigFileNotFoundException(path).what());
	}
}

void ConfigParser::handleIndex(AConfig &config)
{
	std::vector<t_word_line_number_pair> wordLineNumberPairs =
		expectOneOrMoreWords();
	expectSemicolon();
	std::vector<std::string> files;
	for (std::size_t i = 0; i < wordLineNumberPairs.size(); ++i)
	{
		files.push_back(wordLineNumberPairs[i].first);
	}
	config.setIndexes(files);
}

void ConfigParser::handleInternal(AConfig &config)
{
	expectSemicolon();
	config.setWhetherIsInternal(true);
}

void ConfigParser::handleLimitExcept(AConfig &config)
{
	std::vector<t_word_line_number_pair> methodLineNumberPairs =
		expectOneOrMoreWords();
	std::set<HttpMethod> exemptMethods;
	for (std::size_t i = 0; i < methodLineNumberPairs.size(); ++i)
	{
		try
		{
			exemptMethods.insert(HttpMethodHelpers::getHttpMethodForString(
				methodLineNumberPairs[i].first));
		}
		catch (const std::out_of_range &)
		{
			throw std::runtime_error(
				StringBase()
				<< getLocation(methodLineNumberPairs[i].second)
				<< ": Unknown HTTP method \"" << methodLineNumberPairs[i].first
				<< "\" for \"" << currentDirective << "\"");
		}
	}
	expectLeftBrace();
	LimitExceptConfig limitExceptConfig;
	parseDirectives(limitExceptConfig);
	expectRightBrace();
	config.setLimitExcept(LimitExceptValue(exemptMethods, limitExceptConfig));
}

void ConfigParser::handleListen(AConfig &config)
{
	std::size_t rawAddressPortPairLineNumber = currentToken->getLineNumber();
	std::string rawAddressPortPair = expectWord();
	DefaultServerSpecification defaultServerSpecification = NOT_DEFAULT_SERVER;
	while (currentToken->getType() == AConfigToken::WORD)
	{
		std::string option =
			dynamic_cast<WordConfigToken *>(currentToken)->getContent();
		std::size_t optionLineNumber = currentToken->getLineNumber();
		advance();
		if (option == "default_server")
		{
			defaultServerSpecification = DEFAULT_SERVER;
		}
		else
		{
			throw std::runtime_error(
				StringBase()
				<< getLocation(optionLineNumber) << ": Unknown option \""
				<< option << "\" for \"" << currentDirective << "\"");
		}
	}
	expectSemicolon();
	try
	{
		config.addPendingListenAddressPortPair(rawAddressPortPair,
		                                       defaultServerSpecification);
	}
	catch (const std::invalid_argument &e)
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(rawAddressPortPairLineNumber)
		                         << ": Invalid value for \"" << currentDirective
		                         << "\": " << e.what());
	}
}

void ConfigParser::handleLocation(AConfig &config)
{
	std::string firstWord = expectWord();
	std::string path;
	bool isExact = false;
	bool isSuffix = false;
	if (firstWord == "=")
	{
		isExact = true;
		path = expectWord();
	}
	else if (firstWord == "~$")
	{
		isSuffix = true;
		path = expectWord();
	}
	else
	{
		path = firstWord;
	}
	expectLeftBrace();
	LocationConfig locationConfig;
	parseDirectives(locationConfig);
	expectRightBrace();
	if (isExact)
	{
		config.setLocationConfigForExactPath(path, locationConfig);
	}
	else if (isSuffix)
	{
		config.setLocationConfigForPathSuffix(path, locationConfig);
	}
	else
	{
		config.setLocationConfigForPathPrefix(path, locationConfig);
	}
}

void ConfigParser::handleReturn(AConfig &config)
{
	std::size_t firstWordLineNumber = currentToken->getLineNumber();
	std::string firstWord = expectWord();
	try
	{
		if (StringHelpers::isAllDigits(firstWord))
		{
			HttpStatusCode statusCode =
				parseHttpStatusCode(firstWord, firstWordLineNumber);
			if (currentToken->getType() == AConfigToken::WORD)
			{
				std::string extraWord = expectWord();
				expectSemicolon();
				ReturnResponseValue::ResponseType responseType =
					HttpStatusCodeHelpers::isRedirection(statusCode)
						? ReturnResponseValue::REDIRECT_URL
						: ReturnResponseValue::BODY_TEXT;
				config.setReturnResponse(
					ReturnResponseValue(statusCode, responseType, extraWord));
			}
			else
			{
				expectSemicolon();
				config.setReturnResponse(ReturnResponseValue(statusCode));
			}
		}
		else
		{
			expectSemicolon();
			config.setReturnResponse(ReturnResponseValue(firstWord));
		}
	}
	catch (const ReturnResponseValue::InvalidReturnResponseException &e)
	{
		throw std::runtime_error(StringBase()
		                         << getLocation(currentDirectiveLineNumber)
		                         << ": " << e.what());
	}
}

void ConfigParser::handleRoot(AConfig &config)
{
	std::string path = expectWord();
	expectSemicolon();
	config.setRoot(path);
}

void ConfigParser::handleServer(AConfig &config)
{
	expectLeftBrace();
	ServerConfig serverConfig;
	parseDirectives(serverConfig);
	expectRightBrace();
	serverConfig.registerAsServerIn(config);
}

void ConfigParser::handleServerName(AConfig &config)
{
	std::vector<t_word_line_number_pair> serverNameLineNumberPairs =
		expectOneOrMoreWords();
	expectSemicolon();
	std::vector<std::string> serverNames;
	for (std::size_t i = 0; i < serverNameLineNumberPairs.size(); ++i)
	{
		serverNames.push_back(serverNameLineNumberPairs[i].first);
	}
	config.setPendingServerNames(serverNames);
}

void ConfigParser::handleTryFiles(AConfig &config)
{
	std::vector<t_word_line_number_pair> wordLineNumberPairs =
		expectOneOrMoreWords();
	if (wordLineNumberPairs.size() < 2)
	{
		throw std::runtime_error(
			StringBase() << getLocation(currentDirectiveLineNumber) << ": \""
						 << currentDirective
						 << "\" requires at least one file and a fallback");
	}
	expectSemicolon();
	const t_word_line_number_pair &fallbackLineNumberPair =
		wordLineNumberPairs.back();
	std::vector<std::string> files;
	for (std::size_t i = 0; i + 1 < wordLineNumberPairs.size(); ++i)
	{
		files.push_back(wordLineNumberPairs[i].first);
	}
	if (!fallbackLineNumberPair.first.empty() &&
	    fallbackLineNumberPair.first[0] == '=')
	{
		config.setTryFiles(TryFilesValue(
			files, parseHttpStatusCode(fallbackLineNumberPair.first.substr(1),
		                               fallbackLineNumberPair.second)));
	}
	else
	{
		config.setTryFiles(TryFilesValue(files, fallbackLineNumberPair.first));
	}
}

void ConfigParser::handleTypes(AConfig &config)
{
	expectLeftBrace();
	std::map<std::string, std::string> mimeTypesForExtensions;
	while (true)
	{
		if (currentToken->getType() == AConfigToken::TERMINATING)
		{
			throw std::runtime_error(
				StringBase()
				<< getLocation(currentToken->getLineNumber())
				<< ": Expected '}' for \"" << currentDirective << "\"");
		}
		if (currentToken->getType() == AConfigToken::SPECIAL_CHARACTER &&
		    dynamic_cast<SpecialCharacterConfigToken *>(currentToken)
		            ->getContent() == SpecialCharacterConfigToken::RIGHT_BRACE)
		{
			break;
		}
		if (currentToken->getType() != AConfigToken::WORD)
		{
			throw std::runtime_error(
				StringBase() << getLocation(currentToken->getLineNumber())
							 << ": Unexpected token in \"" << currentDirective
							 << "\" block");
		}
		std::string mimeType =
			dynamic_cast<WordConfigToken *>(currentToken)->getContent();
		advance();
		std::vector<t_word_line_number_pair> extensionLineNumberPairs =
			expectOneOrMoreWords();
		expectSemicolon();
		for (std::size_t i = 0; i < extensionLineNumberPairs.size(); ++i)
		{
			if (mimeTypesForExtensions.find(
					extensionLineNumberPairs[i].first) !=
			    mimeTypesForExtensions.end())
			{
				throw std::runtime_error(
					StringBase()
					<< getLocation(extensionLineNumberPairs[i].second)
					<< ": Duplicate extension \""
					<< extensionLineNumberPairs[i].first << "\" in \""
					<< currentDirective << "\" block");
			}
			mimeTypesForExtensions[extensionLineNumberPairs[i].first] =
				mimeType;
		}
	}
	expectRightBrace();
	config.setMimeTypesForExtensions(mimeTypesForExtensions);
}

void ConfigParser::handleUser(AConfig &config)
{
	std::vector<t_word_line_number_pair> wordLineNumberPairs =
		expectOneOrMoreWords();
	if (wordLineNumberPairs.size() > 2)
	{
		throw std::runtime_error(
			StringBase() << getLocation(currentDirectiveLineNumber)
						 << ": Too many values for \"" << currentDirective
						 << "\" (expected at most 2)");
	}
	expectSemicolon();
	const std::string &user = wordLineNumberPairs[0].first;
	config.setWorkerUser(user);
	if (wordLineNumberPairs.size() == 2)
	{
		const std::string &group = wordLineNumberPairs[1].first;
		config.setWorkerGroup(group);
	}
}

void ConfigParser::handleWorkerConnections(AConfig &config)
{
	std::size_t count = expectSize();
	expectSemicolon();
	config.setWorkerMaxConnections(count);
}

void ConfigParser::handleWorkerProcesses(AConfig &config)
{
	std::size_t countLineNumber = currentToken->getLineNumber();
	std::size_t count = expectSize();
	expectSemicolon();
	if (count != 1)
	{
		throw std::runtime_error(
			StringBase() << getLocation(countLineNumber)
						 << ": Multiple worker processes are not currently "
							"supported (\""
						 << currentDirective << "\")");
	}
	config.setWorkerProcesses(count);
}
