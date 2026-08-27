/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 02:54:15 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 12:26:04 by emflynn          ###   ########.fr       */
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
#include "ErrorLogValue.hpp"
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

	void setParentConfig(AConfig *parentConfig);

	bool shouldRunAsDaemonSettingResolves(void) const;
	bool resolveShouldRunAsDaemonSetting(void) const;
	void setWhetherShouldRunAsDaemon(bool shouldRunAsDaemon);

	bool errorLogSettingResolves(void) const;
	const ErrorLogValue &resolveErrorLogSetting(void) const;
	void setErrorLog(const ErrorLogValue &errorLog);

	bool eventsConfigSettingResolves(void) const;
	const EventsConfig &resolveEventsConfigSetting(void) const;
	void setEventsConfig(const EventsConfig &eventsConfig);

	bool workerUserSettingResolves(void) const;
	const std::string &resolveWorkerUserSetting(void) const;
	void setWorkerUser(const std::string &workerUser);

	bool workerGroupSettingResolves(void) const;
	const std::string &resolveWorkerGroupSetting(void) const;
	void setWorkerGroup(const std::string &workerGroup);

	bool workerMaxConnectionsSettingResolves(void) const;
	std::size_t resolveWorkerMaxConnectionsSetting(void) const;
	void setWorkerMaxConnections(std::size_t workerMaxConnections);

	bool workerProcessesSettingResolves(void) const;
	std::size_t resolveWorkerProcessesSetting(void) const;
	void setWorkerProcesses(std::size_t workerProcesses);

	// NOTE: not inherited, resolves at this config level only
	bool aliasSettingResolves(void) const;
	const std::string &resolveAliasSetting(void) const;
	void setAlias(const std::string &alias);

	bool clientMaxBodySizeSettingResolves(void) const;
	std::size_t resolveClientMaxBodySizeSetting(void) const;
	void setClientMaxBodySize(std::size_t clientMaxBodySize);

	bool defaultMimeTypeSettingResolves(void) const;
	const std::string &resolveDefaultMimeTypeSetting(void) const;
	void setDefaultMimeType(const std::string &defaultMimeType);

	bool errorPageSettingResolvesForHttpStatusCode(
		HttpStatusCode statusCode) const;
	const ErrorPageValue &resolveErrorPageSettingForHttpStatusCode(
		HttpStatusCode statusCode) const;
	void setErrorPageForHttpStatusCode(HttpStatusCode statusCode,
	                                   const ErrorPageValue &errorPageValue);

	bool httpConfigSettingResolves(void) const;
	const HttpConfig &resolveHttpConfigSetting(void) const;
	void setHttpConfig(const HttpConfig &httpConfig);

	// NOTE: not inherited, resolves at this config level only
	bool isInternalSettingResolves(void) const;
	bool resolveIsInternalSetting(void) const;
	void setWhetherIsInternal(bool isInternal);

	// NOTE: not inherited, resolves at this config level only
	bool limitExceptConfigSettingResolvesForMethod(HttpMethod method) const;
	const LimitExceptConfig &resolveLimitExceptConfigSettingForMethod(
		HttpMethod method) const;
	void setLimitExcept(const LimitExceptValue &limitExcept);

	bool locationConfigSettingResolvesForPath(const std::string &path) const;
	const LocationConfig &resolveLocationConfigSettingForPath(
		const std::string &path) const;
	void setLocationConfigForExactPath(const std::string &exactPath,
	                                   const LocationConfig &locationConfig);
	void setLocationConfigForPathPrefix(const std::string &pathPrefix,
	                                    const LocationConfig &locationConfig);
	void setLocationConfigForPriorityPathPrefix(
		const std::string &pathPrefix, const LocationConfig &locationConfig);
	void setLocationConfigForPathSuffix(const std::string &pathSuffix,
	                                    const LocationConfig &locationConfig);

	bool rootSettingResolves(void) const;
	const std::string &resolveRootSetting(void) const;
	void setRoot(const std::string &root);

	void addPendingListenAddressPortPair(
		const std::string &rawAddressPortPair,
		DefaultServerSpecification defaultServerSpecification);

	void setPendingServerNames(const std::vector<std::string> &serverNames);

	void registerAsServerIn(AConfig &parentConfig);

	bool anyServerConfigSettingResolves(void) const;
	std::vector<std::string> extractAllServerConfigSettingAddressPortPairs(
		void) const;
	bool serverConfigSettingResolvesForRequestTarget(
		const std::string &addressPortPair,
		const std::string &serverName) const;
	const ServerConfig &resolveServerConfigSettingForRequestTarget(
		const std::string &addressPortPair,
		const std::string &serverName) const;
	void setServerConfig(const ServerConfig &serverConfig);

	bool tryFilesSettingResolves(void) const;
	const TryFilesValue &resolveTryFilesSetting(void) const;
	void setTryFiles(const TryFilesValue &tryFiles);

	bool mimeTypesForExtensionsSettingResolves(void) const;
	const std::map<std::string, std::string> &
	resolveMimeTypesForExtensionsSetting(void) const;
	void setMimeTypesForExtensions(
		const std::map<std::string, std::string> &mimeTypesForExtensions);

	AccessRule resolveAccessRuleSettingForIpAddress(
		const std::string &ipAddress) const;
	void setAccessRuleForIpAddressAndMask(const std::string &ipAddressAndMask,
	                                      AccessRule accessRule);

	bool shouldUseAutoindexSettingResolves(void) const;
	bool resolveShouldUseAutoindexSetting(void) const;
	void setWhetherShouldUseAutoindex(bool shouldUseAutoindex);

	bool indexesSettingResolves(void) const;
	const std::vector<std::string> &resolveIndexesSetting(void) const;
	void setIndexes(const std::vector<std::string> &indexes);

	bool accessLogFilePathSettingResolves(void) const;
	const std::string &resolveAccessLogFilePathSetting(void) const;
	void setAccessLogFilePath(const std::string &accessLogFilePath);

	bool returnResponseSettingResolves(void) const;
	const ReturnResponseValue &resolveReturnResponseSetting(void) const;
	void setReturnResponse(const ReturnResponseValue &returnResponse);

	class ConfigFrozenException: public std::logic_error
	{
	public:
		ConfigFrozenException(void);
	};

	class SettingNotSetException: public std::out_of_range
	{
	public:
		SettingNotSetException(void);
		SettingNotSetException(const std::string &message);
	};

	class DirectiveAlreadySetException: public std::runtime_error
	{
	public:
		DirectiveAlreadySetException(void);
	};

	class ConflictingDirectiveAlreadySetException: public std::runtime_error
	{
	public:
		ConflictingDirectiveAlreadySetException(const std::string &value);
		~ConflictingDirectiveAlreadySetException(void) throw();

		const std::string &getValue(void) const;

	private:
		std::string value;
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

	static void throwIfAlreadySet(bool isAlreadySet);
	static void throwIfConflictingDirectiveAlreadySet(bool isAlreadySet,
	                                                  const std::string &value);
	void throwIfNotSupportedForConfigType(const ConfigType *allowedTypes,
	                                      std::size_t allowedTypesCount) const;

	template<typename TSetting>
	bool settingResolvesAtThisConfigLevelOrAbove(
		OverridableConfigSetting<TSetting> AConfig::*setting) const;
	template<typename TSetting>
	const TSetting &resolveSettingAtThisConfigLevelOrAbove(
		OverridableConfigSetting<TSetting> AConfig::*setting) const;

	const ErrorPageValue *findErrorPageForHttpStatusCode(
		HttpStatusCode statusCode) const;
	const LimitExceptConfig *findLimitExceptConfigForMethod(
		HttpMethod method) const;
	const LocationConfig *findExactLocationConfigForPath(
		const std::string &path) const;
	const LocationConfig *findPrefixLocationConfigForPath(
		const std::string &path) const;
	const LocationConfig *findPriorityPrefixLocationConfigForPath(
		const std::string &path) const;
	const LocationConfig *findSuffixLocationConfigForPath(
		const std::string &path) const;
	const LocationConfig *findLocationConfigForPath(
		const std::string &path) const;
	const ServerConfig *findServerConfigForRequestTarget(
		const std::string &addressPortPair,
		const std::string &serverName) const;

	void setUpFrom(const AConfig &other);
	void tearDown(void);

	void printTo(std::ostream &stream) const;
	void printTo(std::ostream &stream, std::size_t depth) const;

	void registerServerConfigForAddressPortPair(
		const std::string &addressPortPair,
		DefaultServerSpecification defaultServerSpecification,
		const std::vector<std::string> &serverNames,
		const ServerConfig *serverConfig);

	AConfig *parentConfig;

	// From Nginx core functionality
	OverridableConfigSetting<bool> shouldRunAsDaemon;
	OverridableConfigSetting<ErrorLogValue> errorLog;
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
		priorityPrefixLocationConfigs;
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
