/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 03:19:21 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/03 20:04:58 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "ConfigLexer.hpp"
#include "HttpStatusCode.hpp"

class AConfig;
class AConfigToken;

class ConfigParser
{
public:
	ConfigParser(const std::string &configFilePath, AConfig &config);
	~ConfigParser(void);

protected:
	ConfigParser(void);
	ConfigParser(const ConfigParser &other);
	ConfigParser &operator=(const ConfigParser &other);

private:
	ConfigParser(const std::string &configFilePath, AConfig &config,
	             std::set<std::string> *activeIncludes);

	static const std::size_t BYTES_PER_KILOBYTE = 1024;

	typedef void (ConfigParser::*t_handler)(AConfig &);
	typedef std::pair<std::string, std::size_t> t_word_line_number_pair;

	static std::string getAbsolutePath(const std::string &path);
	static std::map<std::string, t_handler> createHandlers(void);

	std::string getLocation(std::size_t lineNumber) const;

	void advance(void);

	std::string expectWord(void);
	void expectSemicolon(void);
	void expectLeftBrace(void);
	void expectRightBrace(void);
	bool expectBool(void);
	std::size_t expectSize(void);
	std::size_t expectSizeWithUnits(void);
	std::vector<t_word_line_number_pair> expectOneOrMoreWords(void);

	void parse(AConfig &config);
	void parseDirectives(AConfig &config);
	std::size_t parseSize(const std::string &value, std::size_t lineNumber);
	HttpStatusCode parseHttpStatusCode(const std::string &value,
	                                   std::size_t lineNumber);

	void handleAccessLog(AConfig &config);
	void handleAlias(AConfig &config);
	void handleAllow(AConfig &config);
	void handleAutoindex(AConfig &config);
	void handleClientMaxBodySize(AConfig &config);
	void handleDaemon(AConfig &config);
	void handleDefaultType(AConfig &config);
	void handleDeny(AConfig &config);
	void handleErrorLog(AConfig &config);
	void handleErrorPage(AConfig &config);
	void handleEvents(AConfig &config);
	void handleHttp(AConfig &config);
	void handleInclude(AConfig &config);
	void handleIndex(AConfig &config);
	void handleInternal(AConfig &config);
	void handleLimitExcept(AConfig &config);
	void handleListen(AConfig &config);
	void handleLocation(AConfig &config);
	void handleReturn(AConfig &config);
	void handleRoot(AConfig &config);
	void handleServer(AConfig &config);
	void handleServerName(AConfig &config);
	void handleTryFiles(AConfig &config);
	void handleTypes(AConfig &config);
	void handleUser(AConfig &config);
	void handleWorkerConnections(AConfig &config);
	void handleWorkerProcesses(AConfig &config);

	ConfigLexer configLexer;
	std::string currentFilePath;
	AConfigToken *currentToken;
	std::string currentDirective;
	std::size_t currentDirectiveLineNumber;
	std::set<std::string> *activeIncludes;
	bool ownsActiveIncludes;
};

#endif
