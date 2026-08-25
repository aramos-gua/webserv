/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 02:54:15 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/25 06:39:01 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_CONFIG_HPP
#define A_CONFIG_HPP

#include <cstddef>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "AccessRule.hpp"
#include "ConfigType.hpp"
#include "DefaultServerSpecification.hpp"
#include "ErrorPageValue.hpp"
#include "HttpMethod.hpp"
#include "HttpStatusCode.hpp"
#include "LimitExceptValue.hpp"
#include "LongestStringFirstComparator.hpp"
#include "MostSpecificMaskFirstComparator.hpp"
#include "OverridableConfigSetting.hpp"
#include "ReturnResponseValue.hpp"
#include "SpecialServerNamesLastComparator.hpp"
#include "TryFilesValue.hpp"

class EventsConfig;
class HttpConfig;
class LocationConfig;
class ServerConfig;

class AConfig
{
public:
	AConfig(void);
	AConfig(const AConfig &other);
	AConfig &operator=(const AConfig &other);
	virtual ~AConfig(void) = 0;

	virtual ConfigType getConfigType(void) const = 0;

	AConfig *getParentConfig(void);
	void setParentConfig(AConfig *parentConfig);

	bool getWhetherShouldRunAsDaemonSet(void) const;
	bool getWhetherShouldRunAsDaemon(void) const;
	void setWhetherShouldRunAsDaemon(bool shouldRunAsDaemon);

	bool getWhetherErrorLogFilePathSet(void) const;
	const std::string &getErrorLogFilePath(void) const;
	void setErrorLogFilePath(const std::string &errorLogFilePath);

	bool getWhetherEventsConfigSet(void) const;
	const EventsConfig &getEventsConfig(void) const;
	void setEventsConfig(const EventsConfig &eventsConfig);

	bool getWhetherWorkerUserSet(void) const;
	const std::string &getWorkerUser(void) const;
	void setWorkerUser(const std::string &workerUser);

	bool getWhetherWorkerGroupSet(void) const;
	const std::string &getWorkerGroup(void) const;
	void setWorkerGroup(const std::string &workerGroup);

	bool getWhetherWorkerMaxConnectionsSet(void) const;
	std::size_t getWorkerMaxConnections(void) const;
	void setWorkerMaxConnections(std::size_t workerMaxConnections);

	bool getWhetherWorkerProcessesSet(void) const;
	std::size_t getWorkerProcesses(void) const;
	void setWorkerProcesses(std::size_t workerProcesses);

	bool getWhetherAliasSet(void) const;
	const std::string &getAlias(void) const;
	void setAlias(const std::string &alias);

	bool getWhetherClientMaxBodySizeSet(void) const;
	std::size_t getClientMaxBodySize(void) const;
	void setClientMaxBodySize(std::size_t clientMaxBodySize);

	bool getWhetherDefaultMimeTypeSet(void) const;
	const std::string &getDefaultMimeType(void) const;
	void setDefaultMimeType(const std::string &defaultMimeType);

	const ErrorPageValue &getErrorPageForHttpStatusCode(
		HttpStatusCode httpStatusCode) const;
	void setErrorPageForHttpStatusCode(HttpStatusCode httpStatusCode,
	                                   const ErrorPageValue &errorPageValue);

	bool getWhetherHttpConfigSet(void) const;
	const HttpConfig &getHttpConfig(void) const;
	void setHttpConfig(const HttpConfig &httpConfig);

	bool getWhetherIsInternalSet(void) const;
	bool getWhetherIsInternal(void) const;
	void setWhetherIsInternal(bool isInternal);

	bool getWhetherLimitExceptSet(void) const;
	const LimitExceptConfig *getLimitExceptConfigForMethod(
		HttpMethod method) const;
	void setLimitExcept(const LimitExceptValue &limitExcept);

	const LocationConfig &getExactLocationConfigForPath(
		const std::string &path) const;
	void setLocationConfigForExactPath(const std::string &exactPath,
	                                   const LocationConfig &locationConfig);

	const LocationConfig &getPrefixLocationConfigForPath(
		const std::string &path) const;
	void setLocationConfigForPathPrefix(const std::string &pathPrefix,
	                                    const LocationConfig &locationConfig);

	const LocationConfig &getSuffixLocationConfigForPath(
		const std::string &path) const;
	void setLocationConfigForPathSuffix(const std::string &pathSuffix,
	                                    const LocationConfig &locationConfig);

	bool getWhetherRootSet(void) const;
	const std::string &getRoot(void) const;
	void setRoot(const std::string &root);

	void addPendingListenAddressPortPair(
		const std::string &rawAddressPortPair,
		DefaultServerSpecification defaultServerSpecification);

	void setPendingServerNames(const std::vector<std::string> &serverNames);

	void registerAsServerIn(AConfig &parentConfig);

	bool getWhetherAnyServerConfigsSet(void) const;
	std::vector<std::string> getServerConfigAddressPortPairs(void) const;
	const ServerConfig &getServerConfig(const std::string &addressPortPair,
	                                    const std::string &serverName) const;
	void setServerConfig(const ServerConfig &serverConfig);

	bool getWhetherTryFilesSet(void) const;
	const TryFilesValue &getTryFiles(void) const;
	void setTryFiles(const TryFilesValue &tryFiles);

	bool getWhetherMimeTypesForExtensionsSet(void) const;
	const std::map<std::string, std::string> &getMimeTypesForExtensions(
		void) const;
	void setMimeTypesForExtensions(
		const std::map<std::string, std::string> &mimeTypesForExtensions);

	AccessRule getAccessRuleForIpAddress(const std::string &ipAddress) const;
	void setAccessRuleForIpAddressAndMask(const std::string &ipAddressAndMask,
	                                      AccessRule accessRule);

	bool getWhetherShouldUseAutoindexSet(void) const;
	bool getWhetherShouldUseAutoindex(void) const;
	void setWhetherShouldUseAutoindex(bool shouldUseAutoindex);

	bool getWhetherIndexesSet(void) const;
	const std::vector<std::string> &getIndexes(void) const;
	void setIndexes(const std::vector<std::string> &indexes);

	bool getWhetherAccessLogFilePathSet(void) const;
	const std::string &getAccessLogFilePath(void) const;
	void setAccessLogFilePath(const std::string &accessLogFilePath);

	bool getWhetherReturnResponseSet(void) const;
	const ReturnResponseValue &getReturnResponse(void) const;
	void setReturnResponse(const ReturnResponseValue &returnResponse);

	class ConfigFrozenException: public std::logic_error
	{
	public:
		ConfigFrozenException(void);
	};

	class DirectiveAlreadySetException: public std::runtime_error
	{
	public:
		DirectiveAlreadySetException(void);
	};

	class DirectiveNotSupportedForConfigTypeException: public std::runtime_error
	{
	public:
		DirectiveNotSupportedForConfigTypeException(void);
	};

	friend std::ostream &operator<<(std::ostream &stream,
	                                const AConfig &config);

protected:
	virtual void throwIfFrozen(void) const;

private:
	typedef std::map<std::string, const ServerConfig *,
	                 SpecialServerNamesLastComparator>
		t_server_configs_by_server_name;
	typedef std::map<std::string, t_server_configs_by_server_name>
		t_server_configs_by_address_port_pair;

	void setUpFrom(const AConfig &other);
	void tearDown(void);

	void printTo(std::ostream &stream) const;
	void printTo(std::ostream &stream, std::size_t depth) const;

	static void throwIfAlreadySet(bool isAlreadySet);
	void throwIfNotSupportedForConfigType(const ConfigType *allowedTypes,
	                                      std::size_t allowedTypesCount) const;

	void registerServerConfigForAddressPortPair(
		const std::string &addressPortPair,
		DefaultServerSpecification defaultServerSpecification,
		const std::vector<std::string> &serverNames,
		const ServerConfig *serverConfig);

	AConfig *parentConfig;

	// From Nginx core functionality
	OverridableConfigSetting<bool> shouldRunAsDaemon;
	OverridableConfigSetting<std::string> errorLogFilePath;
	EventsConfig *eventsConfig;
	OverridableConfigSetting<std::string> workerUser;
	OverridableConfigSetting<std::string> workerGroup;
	OverridableConfigSetting<std::size_t> workerMaxConnections;
	OverridableConfigSetting<std::size_t> workerProcesses;

	// From Nginx ngx_http_core_module
	OverridableConfigSetting<std::string> alias;
	OverridableConfigSetting<std::size_t> clientMaxBodySize;
	OverridableConfigSetting<std::string> defaultMimeType;
	std::map<HttpStatusCode, ErrorPageValue> errorPages;
	HttpConfig *httpConfig;
	OverridableConfigSetting<bool> isInternal;
	OverridableConfigSetting<LimitExceptValue> limitExcept;
	std::map<std::string, LocationConfig *> exactLocationConfigs;
	std::multimap<std::string, LocationConfig *, LongestStringFirstComparator>
		prefixLocationConfigs;
	std::multimap<std::string, LocationConfig *, LongestStringFirstComparator>
		suffixLocationConfigs;
	OverridableConfigSetting<std::string> root;
	std::map<std::string, DefaultServerSpecification>
		pendingListenAddressPortPairs;
	OverridableConfigSetting<std::vector<std::string> > pendingServerNames;
	std::vector<const ServerConfig *> underlyingServerConfigs;
	t_server_configs_by_address_port_pair serverConfigs;
	OverridableConfigSetting<TryFilesValue> tryFiles;
	OverridableConfigSetting<std::map<std::string, std::string> >
		mimeTypesForExtensions;

	// From Nginx ngx_http_access_module
	std::multimap<std::string, AccessRule, MostSpecificMaskFirstComparator>
		accessRules;

	// From Nginx ngx_http_autoindex_module
	OverridableConfigSetting<bool> shouldUseAutoindex;

	// From Nginx ngx_http_index_module
	OverridableConfigSetting<std::vector<std::string> > indexes;

	// From Nginx ngx_http_log_module
	OverridableConfigSetting<std::string> accessLogFilePath;

	// From Nginx ngx_http_rewrite_module
	OverridableConfigSetting<ReturnResponseValue> returnResponse;
};

#endif
