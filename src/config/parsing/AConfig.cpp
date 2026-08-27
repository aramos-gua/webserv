/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 02:54:58 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 12:24:25 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <set>
#include <utility>

#include "AConfig.hpp"
#include "EventsConfig.hpp"
#include "HttpConfig.hpp"
#include "HttpMethodHelpers.hpp"
#include "IpAddressHelpers.hpp"
#include "IpAddressPortPairHelpers.hpp"
#include "LimitExceptConfig.hpp"
#include "LocationConfig.hpp"
#include "LogLevelHelpers.hpp"
#include "PrintableServerConfig.hpp"
#include "ServerConfig.hpp"
#include "ServerNameHelpers.hpp"
#include "SpecialServerNames.hpp"
#include "StringBase.hpp"
#include "StringHelpers.hpp"

AConfig::AConfig(void): parentConfig(NULL), eventsConfig(NULL), httpConfig(NULL)
{
}

AConfig::AConfig(const AConfig &other)
	: parentConfig(other.parentConfig), eventsConfig(NULL), httpConfig(NULL)
{
	setUpFrom(other);
}

AConfig &AConfig::operator=(const AConfig &other)
{
	if (this == &other)
	{
		return *this;
	}
	parentConfig = other.parentConfig;
	tearDown();
	setUpFrom(other);
	return *this;
}

AConfig::~AConfig(void)
{
	tearDown();
}

void AConfig::setParentConfig(AConfig *parentConfig)
{
	this->parentConfig = parentConfig;
}

bool AConfig::shouldRunAsDaemonSettingResolves(void) const
{
	return shouldRunAsDaemon.checkIfSet();
}

bool AConfig::resolveShouldRunAsDaemonSetting(void) const
{
	return shouldRunAsDaemon.get();
}

void AConfig::setWhetherShouldRunAsDaemon(bool shouldRunAsDaemon)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->shouldRunAsDaemon.checkIfSet());
	this->shouldRunAsDaemon.set(shouldRunAsDaemon);
}

bool AConfig::errorLogSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(&AConfig::errorLog);
}

const ErrorLogValue &AConfig::resolveErrorLogSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::errorLog);
}

void AConfig::setErrorLog(const ErrorLogValue &errorLog)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN, HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->errorLog.checkIfSet());
	this->errorLog.set(errorLog);
}

bool AConfig::eventsConfigSettingResolves(void) const
{
	return eventsConfig != NULL;
}

const EventsConfig &AConfig::resolveEventsConfigSetting(void) const
{
	if (eventsConfig == NULL)
	{
		throw SettingNotSetException("No \"events\" block set");
	}
	return *eventsConfig;
}

void AConfig::setEventsConfig(const EventsConfig &eventsConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->eventsConfig != NULL);
	this->eventsConfig = new EventsConfig(eventsConfig);
	this->eventsConfig->setParentConfig(this);
}

bool AConfig::workerUserSettingResolves(void) const
{
	return workerUser.checkIfSet();
}

const std::string &AConfig::resolveWorkerUserSetting(void) const
{
	return workerUser.get();
}

void AConfig::setWorkerUser(const std::string &workerUser)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->workerUser.checkIfSet());
	this->workerUser.set(workerUser);
}

bool AConfig::workerGroupSettingResolves(void) const
{
	return workerGroup.checkIfSet();
}

const std::string &AConfig::resolveWorkerGroupSetting(void) const
{
	return workerGroup.get();
}

void AConfig::setWorkerGroup(const std::string &workerGroup)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->workerGroup.checkIfSet());
	this->workerGroup.set(workerGroup);
}

bool AConfig::workerMaxConnectionsSettingResolves(void) const
{
	return workerMaxConnections.checkIfSet();
}

std::size_t AConfig::resolveWorkerMaxConnectionsSetting(void) const
{
	return workerMaxConnections.get();
}

void AConfig::setWorkerMaxConnections(std::size_t workerMaxConnections)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {EVENTS};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->workerMaxConnections.checkIfSet());
	this->workerMaxConnections.set(workerMaxConnections);
}

bool AConfig::workerProcessesSettingResolves(void) const
{
	return workerProcesses.checkIfSet();
}

std::size_t AConfig::resolveWorkerProcessesSetting(void) const
{
	return workerProcesses.get();
}

void AConfig::setWorkerProcesses(std::size_t workerProcesses)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->workerProcesses.checkIfSet());
	this->workerProcesses.set(workerProcesses);
}

bool AConfig::aliasSettingResolves(void) const
{
	return alias.checkIfSet();
}

const std::string &AConfig::resolveAliasSetting(void) const
{
	return alias.get();
}

void AConfig::setAlias(const std::string &alias)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->alias.checkIfSet());
	this->alias.set(alias);
}

bool AConfig::clientMaxBodySizeSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(&AConfig::clientMaxBodySize);
}

std::size_t AConfig::resolveClientMaxBodySizeSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::clientMaxBodySize);
}

void AConfig::setClientMaxBodySize(std::size_t clientMaxBodySize)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->clientMaxBodySize.checkIfSet());
	this->clientMaxBodySize.set(clientMaxBodySize);
}

bool AConfig::defaultMimeTypeSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(&AConfig::defaultMimeType);
}

const std::string &AConfig::resolveDefaultMimeTypeSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::defaultMimeType);
}

void AConfig::setDefaultMimeType(const std::string &defaultMimeType)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->defaultMimeType.checkIfSet());
	this->defaultMimeType.set(defaultMimeType);
}

bool AConfig::errorPageSettingResolvesForHttpStatusCode(
	HttpStatusCode statusCode) const
{
	return findErrorPageForHttpStatusCode(statusCode) != NULL;
}

const ErrorPageValue &AConfig::resolveErrorPageSettingForHttpStatusCode(
	HttpStatusCode statusCode) const
{
	const ErrorPageValue *errorPage =
		findErrorPageForHttpStatusCode(statusCode);
	if (errorPage == NULL)
	{
		throw SettingNotSetException(
			StringBase() << "No error page configured for status code "
						 << statusCode);
	}
	return *errorPage;
}

const ErrorPageValue *AConfig::findErrorPageForHttpStatusCode(
	HttpStatusCode statusCode) const
{
	for (const AConfig *config = this; config != NULL;
	     config = config->parentConfig)
	{
		std::map<HttpStatusCode, ErrorPageValue>::const_iterator iterator =
			config->errorPages.find(statusCode);
		if (iterator != config->errorPages.end())
		{
			return &iterator->second;
		}
	}
	return NULL;
}

void AConfig::setErrorPageForHttpStatusCode(
	HttpStatusCode statusCode, const ErrorPageValue &errorPageValue)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfConflictingDirectiveAlreadySet(
		errorPages.find(statusCode) != errorPages.end(),
		StringBase() << static_cast<int>(statusCode));
	errorPages[statusCode] = errorPageValue;
}

bool AConfig::httpConfigSettingResolves(void) const
{
	return httpConfig != NULL;
}

const HttpConfig &AConfig::resolveHttpConfigSetting(void) const
{
	if (httpConfig == NULL)
	{
		throw SettingNotSetException("No \"http\" block set");
	}
	return *httpConfig;
}

void AConfig::setHttpConfig(const HttpConfig &httpConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->httpConfig != NULL);
	this->httpConfig = new HttpConfig(httpConfig);
	this->httpConfig->setParentConfig(this);
}

bool AConfig::isInternalSettingResolves(void) const
{
	return isInternal.checkIfSet();
}

bool AConfig::resolveIsInternalSetting(void) const
{
	return isInternal.get();
}

void AConfig::setWhetherIsInternal(bool isInternal)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->isInternal.checkIfSet());
	this->isInternal.set(isInternal);
}

bool AConfig::limitExceptConfigSettingResolvesForMethod(HttpMethod method) const
{
	return findLimitExceptConfigForMethod(method) != NULL;
}

const LimitExceptConfig &AConfig::resolveLimitExceptConfigSettingForMethod(
	HttpMethod method) const
{
	const LimitExceptConfig *limitExceptConfig =
		findLimitExceptConfigForMethod(method);
	if (limitExceptConfig == NULL)
	{
		if (!limitExcept.checkIfSet())
		{
			throw SettingNotSetException("No limitExcept config set at this "
			                             "level");
		}
		throw SettingNotSetException(StringBase()
		                             << "Method " << method
		                             << " unaffected by limitExcept config");
	}
	return *limitExceptConfig;
}

const LimitExceptConfig *AConfig::findLimitExceptConfigForMethod(
	HttpMethod method) const
{
	if (!limitExcept.checkIfSet())
	{
		return NULL;
	}
	const LimitExceptValue &limitExceptValue = limitExcept.get();
	const std::set<HttpMethod> &exemptMethods =
		limitExceptValue.getExemptMethods();
	if (exemptMethods.find(method) != exemptMethods.end())
	{
		return NULL;
	}
	return &limitExceptValue.getLimitExceptConfig();
}

void AConfig::setLimitExcept(const LimitExceptValue &limitExcept)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->limitExcept.checkIfSet());
	this->limitExcept.set(limitExcept);
}

static bool pathPartAlreadyPresent(
	const std::multimap<std::string, LocationConfig *,
                        LongestStringFirstComparator> &locationConfigs,
	const std::string &pathPart)
{
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         locationConfigs.lower_bound(pathPart);
	     iterator != locationConfigs.upper_bound(pathPart); ++iterator)
	{
		if (pathPart == iterator->first)
		{
			return true;
		}
	}
	return false;
}

bool AConfig::locationConfigSettingResolvesForPath(
	const std::string &path) const
{
	return findLocationConfigForPath(path) != NULL;
}

const LocationConfig &AConfig::resolveLocationConfigSettingForPath(
	const std::string &path) const
{
	const LocationConfig *locationConfig = findLocationConfigForPath(path);
	if (locationConfig == NULL)
	{
		throw SettingNotSetException(
			StringBase() << "No matching location config setting for path "
						 << path);
	}
	return *locationConfig;
}

const LocationConfig *AConfig::findLocationConfigForPath(
	const std::string &path) const
{
	const LocationConfig *locationConfig = findExactLocationConfigForPath(path);
	if (locationConfig == NULL)
	{
		locationConfig = findPriorityPrefixLocationConfigForPath(path);
	}
	if (locationConfig == NULL)
	{
		locationConfig = findSuffixLocationConfigForPath(path);
	}
	if (locationConfig == NULL)
	{
		locationConfig = findPrefixLocationConfigForPath(path);
	}
	if (locationConfig == NULL)
	{
		return NULL;
	}
	const LocationConfig *nestedLocationConfig =
		locationConfig->findLocationConfigForPath(path);
	return nestedLocationConfig != NULL ? nestedLocationConfig : locationConfig;
}

const LocationConfig *AConfig::findExactLocationConfigForPath(
	const std::string &path) const
{
	std::map<std::string, LocationConfig *>::const_iterator iterator =
		exactLocationConfigs.find(path);
	if (iterator == exactLocationConfigs.end())
	{
		return NULL;
	}
	return iterator->second;
}

void AConfig::setLocationConfigForExactPath(
	const std::string &exactPath, const LocationConfig &locationConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	std::pair<std::map<std::string, LocationConfig *>::iterator, bool>
		insertionResult = exactLocationConfigs.insert(
			std::pair<std::string, LocationConfig *>(exactPath, NULL));
	bool wasNewlyInserted = insertionResult.second;
	throwIfConflictingDirectiveAlreadySet(!wasNewlyInserted, exactPath);
	std::map<std::string, LocationConfig *>::iterator
		newlyInsertedLocationConfigIterator = insertionResult.first;
	newlyInsertedLocationConfigIterator->second =
		new LocationConfig(locationConfig);
	newlyInsertedLocationConfigIterator->second->setParentConfig(this);
}

const LocationConfig *AConfig::findPrefixLocationConfigForPath(
	const std::string &path) const
{
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         prefixLocationConfigs.lower_bound(path);
	     iterator != prefixLocationConfigs.end(); ++iterator)
	{
		if (!path.compare(0, iterator->first.size(), iterator->first))
		{
			return iterator->second;
		}
	}
	return NULL;
}

void AConfig::setLocationConfigForPathPrefix(
	const std::string &pathPrefix, const LocationConfig &locationConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfConflictingDirectiveAlreadySet(
		pathPartAlreadyPresent(prefixLocationConfigs, pathPrefix) ||
			pathPartAlreadyPresent(priorityPrefixLocationConfigs, pathPrefix),
		pathPrefix);
	prefixLocationConfigs
		.insert(std::pair<std::string, LocationConfig *>(
			pathPrefix, new LocationConfig(locationConfig)))
		->second->setParentConfig(this);
}

const LocationConfig *AConfig::findPriorityPrefixLocationConfigForPath(
	const std::string &path) const
{
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         priorityPrefixLocationConfigs.lower_bound(path);
	     iterator != priorityPrefixLocationConfigs.end(); ++iterator)
	{
		if (!path.compare(0, iterator->first.size(), iterator->first))
		{
			return iterator->second;
		}
	}
	return NULL;
}

void AConfig::setLocationConfigForPriorityPathPrefix(
	const std::string &pathPrefix, const LocationConfig &locationConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfConflictingDirectiveAlreadySet(
		pathPartAlreadyPresent(priorityPrefixLocationConfigs, pathPrefix) ||
			pathPartAlreadyPresent(prefixLocationConfigs, pathPrefix),
		pathPrefix);
	priorityPrefixLocationConfigs
		.insert(std::pair<std::string, LocationConfig *>(
			pathPrefix, new LocationConfig(locationConfig)))
		->second->setParentConfig(this);
}

const LocationConfig *AConfig::findSuffixLocationConfigForPath(
	const std::string &path) const
{
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         suffixLocationConfigs.lower_bound(path);
	     iterator != suffixLocationConfigs.end(); ++iterator)
	{
		const std::string &pathSuffix = iterator->first;
		if (pathSuffix.size() <= path.size() &&
		    !path.compare(path.size() - pathSuffix.size(), pathSuffix.size(),
		                  pathSuffix))
		{
			return iterator->second;
		}
	}
	return NULL;
}

void AConfig::setLocationConfigForPathSuffix(
	const std::string &pathSuffix, const LocationConfig &locationConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfConflictingDirectiveAlreadySet(
		pathPartAlreadyPresent(suffixLocationConfigs, pathSuffix), pathSuffix);
	suffixLocationConfigs
		.insert(std::pair<std::string, LocationConfig *>(
			pathSuffix, new LocationConfig(locationConfig)))
		->second->setParentConfig(this);
}

bool AConfig::rootSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(&AConfig::root);
}

const std::string &AConfig::resolveRootSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::root);
}

void AConfig::setRoot(const std::string &root)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->root.checkIfSet());
	this->root.set(root);
}

void AConfig::addPendingListenAddressPortPair(
	const std::string &rawAddressPortPair,
	DefaultServerSpecification defaultServerSpecification)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	std::vector<std::string> canonicalPairs =
		IpAddressPortPairHelpers::getCanonicalIpAddressPortPairs(
			rawAddressPortPair);
	for (std::size_t i = 0; i < canonicalPairs.size(); ++i)
	{
		throwIfConflictingDirectiveAlreadySet(
			pendingListenAddressPortPairs.find(canonicalPairs[i]) !=
				pendingListenAddressPortPairs.end(),
			canonicalPairs[i]);
		pendingListenAddressPortPairs[canonicalPairs[i]] =
			defaultServerSpecification;
	}
}

void AConfig::setPendingServerNames(const std::vector<std::string> &serverNames)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->pendingServerNames.checkIfSet());
	pendingServerNames.set(serverNames);
}

void AConfig::registerAsServerIn(AConfig &parentConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	parentConfig.setServerConfig(*dynamic_cast<ServerConfig *>(this));
}

bool AConfig::anyServerConfigSettingResolves(void) const
{
	return !serverConfigs.empty();
}

std::vector<std::string> AConfig::extractAllServerConfigSettingAddressPortPairs(
	void) const
{
	std::vector<std::string> addressPortPairs;
	for (t_server_configs_by_address_port_pair::const_iterator iterator =
	         serverConfigs.begin();
	     iterator != serverConfigs.end(); ++iterator)
	{
		addressPortPairs.push_back(iterator->first);
	}
	return addressPortPairs;
}

bool AConfig::serverConfigSettingResolvesForRequestTarget(
	const std::string &addressPortPair, const std::string &serverName) const
{
	return findServerConfigForRequestTarget(addressPortPair, serverName) !=
	       NULL;
}

const ServerConfig &AConfig::resolveServerConfigSettingForRequestTarget(
	const std::string &addressPortPair, const std::string &serverName) const
{
	const ServerConfig *serverConfig =
		findServerConfigForRequestTarget(addressPortPair, serverName);
	if (serverConfig == NULL)
	{
		if (serverConfigs.find(addressPortPair) == serverConfigs.end())
		{
			throw SettingNotSetException(StringBase()
			                             << "Nothing configured to listen on "
			                             << addressPortPair);
		}
		throw SettingNotSetException(
			StringBase() << "No server configured for " << addressPortPair
						 << " and server name \"" << serverName << "\"");
	}
	return *serverConfig;
}

const ServerConfig *AConfig::findServerConfigForRequestTarget(
	const std::string &addressPortPair, const std::string &serverName) const
{
	t_server_configs_by_address_port_pair::const_iterator
		addressPortPairIterator = serverConfigs.find(addressPortPair);
	if (addressPortPairIterator == serverConfigs.end())
	{
		return NULL;
	}
	const t_server_configs_by_server_name &serverConfigsForServerNames =
		addressPortPairIterator->second;
	t_server_configs_by_server_name::const_iterator iterator =
		serverConfigsForServerNames.find(serverName);
	if (iterator == serverConfigsForServerNames.end())
	{
		iterator =
			serverConfigsForServerNames.find(SpecialServerNames::DEFAULT);
	}
	if (iterator == serverConfigsForServerNames.end())
	{
		iterator = serverConfigsForServerNames.find(SpecialServerNames::FIRST);
	}
	if (iterator == serverConfigsForServerNames.end())
	{
		return NULL;
	}
	return iterator->second;
}

void AConfig::setServerConfig(const ServerConfig &serverConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	if (serverConfig.pendingListenAddressPortPairs.empty())
	{
		throw std::runtime_error(
			"\"listen\" directive is required in \"server\" context");
	}
	std::vector<std::string> serverNames =
		serverConfig.pendingServerNames.checkIfSet()
			? serverConfig.pendingServerNames.get()
			: std::vector<std::string>(1, std::string());
	ServerConfig *serverConfigCopy = new ServerConfig(serverConfig);
	serverConfigCopy->setParentConfig(this);
	underlyingServerConfigs.push_back(serverConfigCopy);
	const ServerConfig *latestServerConfig = serverConfigCopy;
	for (std::map<std::string, DefaultServerSpecification>::const_iterator
	         addressPortPairIterator =
	             serverConfig.pendingListenAddressPortPairs.begin();
	     addressPortPairIterator !=
	     serverConfig.pendingListenAddressPortPairs.end();
	     ++addressPortPairIterator)
	{
		registerServerConfigForAddressPortPair(addressPortPairIterator->first,
		                                       addressPortPairIterator->second,
		                                       serverNames, latestServerConfig);
	}
}

bool AConfig::tryFilesSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(&AConfig::tryFiles);
}

const TryFilesValue &AConfig::resolveTryFilesSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::tryFiles);
}

void AConfig::setTryFiles(const TryFilesValue &tryFiles)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->tryFiles.checkIfSet());
	this->tryFiles.set(tryFiles);
}

bool AConfig::mimeTypesForExtensionsSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(
		&AConfig::mimeTypesForExtensions);
}

const std::map<std::string, std::string> &AConfig::
	resolveMimeTypesForExtensionsSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(
		&AConfig::mimeTypesForExtensions);
}

void AConfig::setMimeTypesForExtensions(
	const std::map<std::string, std::string> &mimeTypesForExtensions)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->mimeTypesForExtensions.checkIfSet());
	this->mimeTypesForExtensions.set(mimeTypesForExtensions);
}

AccessRule AConfig::resolveAccessRuleSettingForIpAddress(
	const std::string &ipAddress) const
{
	for (std::multimap<std::string, AccessRule,
	                   MostSpecificMaskFirstComparator>::const_iterator
	         iterator = accessRules.begin();
	     iterator != accessRules.end(); ++iterator)
	{
		const std::string &ruleIpAddressAndMask = iterator->first;
		AccessRule accessRule = iterator->second;
		if (IpAddressHelpers::ipAddressBelongsToSubnet(ipAddress,
		                                               ruleIpAddressAndMask))
		{
			return accessRule;
		}
	}
	if (parentConfig)
	{
		return parentConfig->resolveAccessRuleSettingForIpAddress(ipAddress);
	}
	return ALLOW;
}

void AConfig::setAccessRuleForIpAddressAndMask(
	const std::string &ipAddressAndMask, AccessRule accessRule)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION,
	                                           LIMIT_EXCEPT};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	if (ipAddressAndMask == "all")
	{
		setAccessRuleForIpAddressAndMask("0.0.0.0/0", accessRule);
		setAccessRuleForIpAddressAndMask("::/0", accessRule);
		return;
	}
	if (!IpAddressHelpers::isValidIpAddressAndMask(ipAddressAndMask))
	{
		throw std::invalid_argument(
			StringBase() << "Invalid IP address or mask in access rule: "
						 << ipAddressAndMask);
	}
	std::string canonicalKey =
		IpAddressHelpers::canonicaliseIpAddressAndMask(ipAddressAndMask);
	for (std::multimap<std::string, AccessRule,
	                   MostSpecificMaskFirstComparator>::iterator iterator =
	         accessRules.lower_bound(canonicalKey);
	     iterator != accessRules.upper_bound(canonicalKey); ++iterator)
	{
		if (canonicalKey == iterator->first)
		{
			throwIfConflictingDirectiveAlreadySet(true, canonicalKey);
		}
	}
	accessRules.insert(
		std::pair<std::string, AccessRule>(canonicalKey, accessRule));
}

bool AConfig::shouldUseAutoindexSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(
		&AConfig::shouldUseAutoindex);
}

bool AConfig::resolveShouldUseAutoindexSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::shouldUseAutoindex);
}

void AConfig::setWhetherShouldUseAutoindex(bool shouldUseAutoindex)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->shouldUseAutoindex.checkIfSet());
	this->shouldUseAutoindex.set(shouldUseAutoindex);
}

bool AConfig::indexesSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(&AConfig::indexes);
}

const std::vector<std::string> &AConfig::resolveIndexesSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::indexes);
}

void AConfig::setIndexes(const std::vector<std::string> &indexes)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->indexes.checkIfSet());
	this->indexes.set(indexes);
}

bool AConfig::accessLogFilePathSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(&AConfig::accessLogFilePath);
}

const std::string &AConfig::resolveAccessLogFilePathSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::accessLogFilePath);
}

void AConfig::setAccessLogFilePath(const std::string &accessLogFilePath)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION,
	                                           LIMIT_EXCEPT};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->accessLogFilePath.checkIfSet());
	this->accessLogFilePath.set(accessLogFilePath);
}

bool AConfig::returnResponseSettingResolves(void) const
{
	return settingResolvesAtThisConfigLevelOrAbove(&AConfig::returnResponse);
}

const ReturnResponseValue &AConfig::resolveReturnResponseSetting(void) const
{
	return resolveSettingAtThisConfigLevelOrAbove(&AConfig::returnResponse);
}

void AConfig::setReturnResponse(const ReturnResponseValue &returnResponse)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(this->returnResponse.checkIfSet());
	this->returnResponse.set(returnResponse);
}

void AConfig::throwIfFrozen(void) const
{
	if (parentConfig)
	{
		parentConfig->throwIfFrozen();
	}
}

void AConfig::throwIfAlreadySet(bool isAlreadySet)
{
	if (isAlreadySet)
	{
		throw DirectiveAlreadySetException();
	}
}

void AConfig::throwIfConflictingDirectiveAlreadySet(bool isAlreadySet,
                                                    const std::string &value)
{
	if (isAlreadySet)
	{
		throw ConflictingDirectiveAlreadySetException(value);
	}
}

void AConfig::throwIfNotSupportedForConfigType(
	const ConfigType *allowedTypes, std::size_t allowedTypesCount) const
{
	for (std::size_t i = 0; i < allowedTypesCount; ++i)
	{
		if (allowedTypes[i] == getConfigType())
		{
			return;
		}
	}
	throw DirectiveNotSupportedForConfigTypeException();
}

template<typename TSetting>
bool AConfig::settingResolvesAtThisConfigLevelOrAbove(
	OverridableConfigSetting<TSetting> AConfig::*setting) const
{
	for (const AConfig *config = this; config != NULL;
	     config = config->parentConfig)
	{
		if ((config->*setting).checkIfSet())
		{
			return true;
		}
	}
	return false;
}

template<typename TSetting>
const TSetting &AConfig::resolveSettingAtThisConfigLevelOrAbove(
	OverridableConfigSetting<TSetting> AConfig::*setting) const
{
	for (const AConfig *config = this; config != NULL;
	     config = config->parentConfig)
	{
		if ((config->*setting).checkIfSet())
		{
			return (config->*setting).get();
		}
	}
	throw SettingNotSetException();
}

void AConfig::setUpFrom(const AConfig &other)
{
	shouldRunAsDaemon = other.shouldRunAsDaemon;
	errorLog = other.errorLog;
	workerUser = other.workerUser;
	workerGroup = other.workerGroup;
	workerMaxConnections = other.workerMaxConnections;
	workerProcesses = other.workerProcesses;
	alias = other.alias;
	clientMaxBodySize = other.clientMaxBodySize;
	defaultMimeType = other.defaultMimeType;
	errorPages = other.errorPages;
	isInternal = other.isInternal;
	limitExcept = other.limitExcept;
	root = other.root;
	pendingListenAddressPortPairs = other.pendingListenAddressPortPairs;
	pendingServerNames = other.pendingServerNames;
	tryFiles = other.tryFiles;
	mimeTypesForExtensions = other.mimeTypesForExtensions;
	accessRules = other.accessRules;
	shouldUseAutoindex = other.shouldUseAutoindex;
	indexes = other.indexes;
	accessLogFilePath = other.accessLogFilePath;
	returnResponse = other.returnResponse;
	eventsConfig =
		other.eventsConfig ? new EventsConfig(*other.eventsConfig) : NULL;
	if (eventsConfig)
	{
		eventsConfig->setParentConfig(this);
	}
	httpConfig = other.httpConfig ? new HttpConfig(*other.httpConfig) : NULL;
	if (httpConfig)
	{
		httpConfig->setParentConfig(this);
	}
	for (std::map<std::string, LocationConfig *>::const_iterator iterator =
	         other.exactLocationConfigs.begin();
	     iterator != other.exactLocationConfigs.end(); ++iterator)
	{
		exactLocationConfigs[iterator->first] =
			new LocationConfig(*iterator->second);
		exactLocationConfigs[iterator->first]->setParentConfig(this);
	}
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         other.prefixLocationConfigs.begin();
	     iterator != other.prefixLocationConfigs.end(); ++iterator)
	{
		prefixLocationConfigs
			.insert(std::pair<std::string, LocationConfig *>(
				iterator->first, new LocationConfig(*iterator->second)))
			->second->setParentConfig(this);
	}
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         other.priorityPrefixLocationConfigs.begin();
	     iterator != other.priorityPrefixLocationConfigs.end(); ++iterator)
	{
		priorityPrefixLocationConfigs
			.insert(std::pair<std::string, LocationConfig *>(
				iterator->first, new LocationConfig(*iterator->second)))
			->second->setParentConfig(this);
	}
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         other.suffixLocationConfigs.begin();
	     iterator != other.suffixLocationConfigs.end(); ++iterator)
	{
		suffixLocationConfigs
			.insert(std::pair<std::string, LocationConfig *>(
				iterator->first, new LocationConfig(*iterator->second)))
			->second->setParentConfig(this);
	}
	std::map<const ServerConfig *, ServerConfig *> serverConfigPtrMap;
	for (std::vector<const ServerConfig *>::const_iterator iterator =
	         other.underlyingServerConfigs.begin();
	     iterator != other.underlyingServerConfigs.end(); ++iterator)
	{
		ServerConfig *serverConfigCopy = new ServerConfig(**iterator);
		serverConfigCopy->setParentConfig(this);
		underlyingServerConfigs.push_back(serverConfigCopy);
		serverConfigPtrMap[*iterator] = serverConfigCopy;
	}
	for (t_server_configs_by_address_port_pair::const_iterator outerIterator =
	         other.serverConfigs.begin();
	     outerIterator != other.serverConfigs.end(); ++outerIterator)
	{
		for (t_server_configs_by_server_name::const_iterator innerIterator =
		         outerIterator->second.begin();
		     innerIterator != outerIterator->second.end(); ++innerIterator)
		{
			serverConfigs[outerIterator->first][innerIterator->first] =
				serverConfigPtrMap[innerIterator->second];
		}
	}
}

void AConfig::tearDown(void)
{
	delete eventsConfig;
	delete httpConfig;
	for (std::map<std::string, LocationConfig *>::iterator iterator =
	         exactLocationConfigs.begin();
	     iterator != exactLocationConfigs.end(); ++iterator)
	{
		delete iterator->second;
	}
	exactLocationConfigs.clear();
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::iterator iterator =
	         prefixLocationConfigs.begin();
	     iterator != prefixLocationConfigs.end(); ++iterator)
	{
		delete iterator->second;
	}
	prefixLocationConfigs.clear();
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::iterator iterator =
	         priorityPrefixLocationConfigs.begin();
	     iterator != priorityPrefixLocationConfigs.end(); ++iterator)
	{
		delete iterator->second;
	}
	priorityPrefixLocationConfigs.clear();
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::iterator iterator =
	         suffixLocationConfigs.begin();
	     iterator != suffixLocationConfigs.end(); ++iterator)
	{
		delete iterator->second;
	}
	suffixLocationConfigs.clear();
	for (std::vector<const ServerConfig *>::iterator iterator =
	         underlyingServerConfigs.begin();
	     iterator != underlyingServerConfigs.end(); ++iterator)
	{
		delete *iterator;
	}
	underlyingServerConfigs.clear();
	serverConfigs.clear();
}

void AConfig::registerServerConfigForAddressPortPair(
	const std::string &addressPortPair,
	DefaultServerSpecification defaultServerSpecification,
	const std::vector<std::string> &serverNames,
	const ServerConfig *serverConfig)
{
	t_server_configs_by_address_port_pair::iterator addressPortPairIterator =
		serverConfigs.find(addressPortPair);
	if (addressPortPairIterator == serverConfigs.end())
	{
		addressPortPairIterator =
			serverConfigs
				.insert(std::make_pair(addressPortPair,
		                               t_server_configs_by_server_name()))
				.first;
		addressPortPairIterator->second[SpecialServerNames::FIRST] =
			serverConfig;
	}
	t_server_configs_by_server_name &serverConfigsForAddressPortPair =
		addressPortPairIterator->second;
	if (defaultServerSpecification == DEFAULT_SERVER)
	{
		if (serverConfigsForAddressPortPair.find(SpecialServerNames::DEFAULT) !=
		    serverConfigsForAddressPortPair.end())
		{
			throw std::runtime_error(StringBase()
			                         << "More than one default server set for "
			                         << addressPortPair);
		}
		serverConfigsForAddressPortPair[SpecialServerNames::DEFAULT] =
			serverConfig;
	}
	for (std::vector<std::string>::const_iterator serverNameIterator =
	         serverNames.begin();
	     serverNameIterator != serverNames.end(); ++serverNameIterator)
	{
		const std::string serverName =
			ServerNameHelpers::isValidServerName(*serverNameIterator)
				? StringHelpers::toLowercase(*serverNameIterator)
				: SpecialServerNames::CATCH_ALL;
		t_server_configs_by_server_name::const_iterator existingIterator =
			serverConfigsForAddressPortPair.find(serverName);
		if (existingIterator != serverConfigsForAddressPortPair.end() &&
		    existingIterator->second != serverConfig)
		{
			throw std::runtime_error(
				StringBase()
				<< "More than one server definition provided for "
				<< addressPortPair << " for server with "
				<< (serverName == SpecialServerNames::CATCH_ALL
			            ? "fallback name"
			            : ("(lowercase) name " +
			               (serverName.empty() ? "\"\"" : serverName))));
		}
		serverConfigsForAddressPortPair[serverName] = serverConfig;
	}
}

AConfig::ConfigFrozenException::ConfigFrozenException(void)
	: std::logic_error("Cannot modify config once it has been frozen")
{
}

AConfig::SettingNotSetException::SettingNotSetException(void)
	: std::out_of_range("Cannot read a config setting that is set at no level")
{
}

AConfig::SettingNotSetException::SettingNotSetException(
	const std::string &message)
	: std::out_of_range(message)
{
}

AConfig::DirectiveAlreadySetException::DirectiveAlreadySetException(void)
	: std::runtime_error("Directive already set for this context")
{
}

AConfig::ConflictingDirectiveAlreadySetException::
	ConflictingDirectiveAlreadySetException(const std::string &value)
	: std::runtime_error(StringBase()
                         << "Conflicting directive already set with value \""
                         << value << "\" for this context"),
	  value(value)
{
}

AConfig::ConflictingDirectiveAlreadySetException::
	~ConflictingDirectiveAlreadySetException(void) throw()
{
}

const std::string &AConfig::ConflictingDirectiveAlreadySetException::getValue(
	void) const
{
	return value;
}

AConfig::DirectiveNotSupportedForConfigTypeException::
	DirectiveNotSupportedForConfigTypeException(void)
	: std::runtime_error("Directive not supported for this config type")
{
}

void AConfig::printTo(std::ostream &stream) const
{
	static const std::size_t STARTING_DEPTH = 0;
	printTo(stream, STARTING_DEPTH);
}

void AConfig::printTo(std::ostream &stream, std::size_t depth) const
{
	static const std::size_t INDENT_SIZE = 4;
	std::string indent(depth * INDENT_SIZE, ' ');

	if (accessLogFilePath.checkIfSet())
	{
		stream << indent << "access_log " << accessLogFilePath.get() << ";"
			   << std::endl;
	}

	if (alias.checkIfSet())
	{
		stream << indent << "alias " << alias.get() << ";" << std::endl;
	}

	for (std::multimap<std::string, AccessRule,
	                   MostSpecificMaskFirstComparator>::const_iterator
	         iterator = accessRules.begin();
	     iterator != accessRules.end(); ++iterator)
	{
		stream << indent << (iterator->second == ALLOW ? "allow" : "deny")
			   << " " << iterator->first << ";" << std::endl;
	}

	if (shouldUseAutoindex.checkIfSet())
	{
		stream << indent << "autoindex "
			   << (shouldUseAutoindex.get() ? "on" : "off") << ";" << std::endl;
	}

	if (clientMaxBodySize.checkIfSet())
	{
		stream << indent << "client_max_body_size " << clientMaxBodySize.get()
			   << ";" << std::endl;
	}

	if (shouldRunAsDaemon.checkIfSet())
	{
		stream << indent << "daemon "
			   << (shouldRunAsDaemon.get() ? "on" : "off") << ";" << std::endl;
	}

	if (defaultMimeType.checkIfSet())
	{
		stream << indent << "default_type " << defaultMimeType.get() << ";"
			   << std::endl;
	}

	if (errorLog.checkIfSet())
	{
		const ErrorLogValue &errorLogValue = errorLog.get();
		stream << indent << "error_log " << errorLogValue.getFilePath();
		if (errorLogValue.getWhetherLogLevelSpecified())
		{
			stream << " "
				   << LogLevelHelpers::getStringForLogLevel(
						  errorLogValue.getLogLevel());
		}
		stream << ";" << std::endl;
	}

	for (std::map<HttpStatusCode, ErrorPageValue>::const_iterator iterator =
	         errorPages.begin();
	     iterator != errorPages.end(); ++iterator)
	{
		stream << indent << "error_page " << static_cast<int>(iterator->first);
		HttpStatusCode returnCode =
			iterator->second.getHttpStatusCodeToReturn();
		if (returnCode != iterator->first)
		{
			stream << " =";
			if (returnCode != NONE)
			{
				stream << static_cast<int>(returnCode);
			}
		}
		stream << " " << iterator->second.getUri() << ";" << std::endl;
	}

	if (indexes.checkIfSet())
	{
		stream << indent << "index";
		const std::vector<std::string> &indexValues = indexes.get();
		for (std::size_t i = 0; i < indexValues.size(); ++i)
		{
			stream << " " << indexValues[i];
		}
		stream << ";" << std::endl;
	}

	if (isInternal.checkIfSet() && isInternal.get())
	{
		stream << indent << "internal;" << std::endl;
	}

	if (returnResponse.checkIfSet())
	{
		const ReturnResponseValue &returnValue = returnResponse.get();
		stream << indent << "return "
			   << static_cast<int>(returnValue.getHttpStatusCode());
		if (returnValue.getResponseType() == ReturnResponseValue::BODY_TEXT)
		{
			stream << " " << returnValue.getBodyText();
		}
		else if (returnValue.getResponseType() ==
		         ReturnResponseValue::REDIRECT_URL)
		{
			stream << " " << returnValue.getRedirectUrl();
		}
		stream << ";" << std::endl;
	}

	if (root.checkIfSet())
	{
		stream << indent << "root " << root.get() << ";" << std::endl;
	}

	if (tryFiles.checkIfSet())
	{
		const TryFilesValue &tryFilesValue = tryFiles.get();
		stream << indent << "try_files";
		const std::vector<std::string> &filePaths = tryFilesValue.getFiles();
		for (std::size_t i = 0; i < filePaths.size(); ++i)
		{
			stream << " " << filePaths[i];
		}
		if (tryFilesValue.getFallbackType() == TryFilesValue::URL)
		{
			stream << " " << tryFilesValue.getFallbackUrl();
		}
		else
		{
			stream << " ="
				   << static_cast<int>(
						  tryFilesValue.getFallbackHttpStatusCode());
		}
		stream << ";" << std::endl;
	}

	if (workerUser.checkIfSet())
	{
		stream << indent << "user " << workerUser.get();
		if (workerGroup.checkIfSet())
		{
			stream << " " << workerGroup.get();
		}
		stream << ";" << std::endl;
	}

	if (workerMaxConnections.checkIfSet())
	{
		stream << indent << "worker_connections " << workerMaxConnections.get()
			   << ";" << std::endl;
	}

	if (workerProcesses.checkIfSet())
	{
		stream << indent << "worker_processes " << workerProcesses.get() << ";"
			   << std::endl;
	}

	if (eventsConfig != NULL)
	{
		stream << indent << "events {" << std::endl;
		eventsConfig->printTo(stream, depth + 1);
		stream << indent << "}" << std::endl;
	}

	if (mimeTypesForExtensions.checkIfSet())
	{
		const std::map<std::string, std::string> &mimeTypeByExtension =
			mimeTypesForExtensions.get();
		std::map<std::string, std::vector<std::string> > extensionsByMimeType;
		for (std::map<std::string, std::string>::const_iterator iterator =
		         mimeTypeByExtension.begin();
		     iterator != mimeTypeByExtension.end(); ++iterator)
		{
			extensionsByMimeType[iterator->second].push_back(iterator->first);
		}
		stream << indent << "types {" << std::endl;
		for (std::map<std::string, std::vector<std::string> >::const_iterator
		         iterator = extensionsByMimeType.begin();
		     iterator != extensionsByMimeType.end(); ++iterator)
		{
			stream << indent << indent << iterator->first;
			for (std::size_t i = 0; i < iterator->second.size(); ++i)
			{
				stream << " " << iterator->second[i];
			}
			stream << ";" << std::endl;
		}
		stream << indent << "}" << std::endl;
	}

	if (httpConfig != NULL)
	{
		stream << indent << "http {" << std::endl;
		httpConfig->printTo(stream, depth + 1);
		stream << indent << "}" << std::endl;
	}

	std::map<const ServerConfig *, PrintableServerConfig>
		printableServerConfigs;
	std::vector<const ServerConfig *> serverConfigsInPrintOrder;
	for (t_server_configs_by_address_port_pair::const_iterator
	         addressPortPairIterator = serverConfigs.begin();
	     addressPortPairIterator != serverConfigs.end();
	     ++addressPortPairIterator)
	{
		const std::string &addressPortPair = addressPortPairIterator->first;
		const t_server_configs_by_server_name &serverConfigsForAddressPortPair =
			addressPortPairIterator->second;
		bool hasExplicitDefaultServer =
			serverConfigsForAddressPortPair.find(SpecialServerNames::DEFAULT) !=
			serverConfigsForAddressPortPair.end();
		for (t_server_configs_by_server_name::const_iterator
		         serverNameIterator = serverConfigsForAddressPortPair.begin();
		     serverNameIterator != serverConfigsForAddressPortPair.end();
		     ++serverNameIterator)
		{
			const std::string &serverName = serverNameIterator->first;
			const ServerConfig *serverConfig = serverNameIterator->second;
			if (printableServerConfigs.find(serverConfig) ==
			    printableServerConfigs.end())
			{
				serverConfigsInPrintOrder.push_back(serverConfig);
			}
			// NOTE: std::map's operator[] method will default-construct a
			// PrintableServerConfig for this const ServerConfig * if one
			// isn't already present. Otherwise, it will use the existing one
			PrintableServerConfig &printableServerConfig =
				printableServerConfigs[serverConfig];
			printableServerConfig.addAddressPortPair(addressPortPair);
			if (serverName == SpecialServerNames::DEFAULT ||
			    (serverName == SpecialServerNames::FIRST &&
			     !hasExplicitDefaultServer))
			{
				printableServerConfig.setAddressPortPairAsDefaultServer(
					addressPortPair);
			}
			else if (serverName != SpecialServerNames::FIRST &&
			         serverName != SpecialServerNames::CATCH_ALL)
			{
				printableServerConfig.addServerName(serverName);
			}
		}
	}

	for (std::vector<const ServerConfig *>::const_iterator
	         serverConfigIterator = serverConfigsInPrintOrder.begin();
	     serverConfigIterator != serverConfigsInPrintOrder.end();
	     ++serverConfigIterator)
	{
		const ServerConfig *serverConfig = *serverConfigIterator;
		const PrintableServerConfig &printableServerConfig =
			printableServerConfigs[serverConfig];
		stream << indent << "server {" << std::endl;
		const std::map<std::string, bool> &defaultServerFlagsByAddressPortPair =
			printableServerConfig.getDefaultServerFlagsByAddressPortPair();
		for (std::map<std::string, bool>::const_iterator flagIterator =
		         defaultServerFlagsByAddressPortPair.begin();
		     flagIterator != defaultServerFlagsByAddressPortPair.end();
		     ++flagIterator)
		{
			const std::string &addressPortPair = flagIterator->first;
			bool isDefaultServer = flagIterator->second;
			stream << indent << indent << "listen " << addressPortPair;
			if (isDefaultServer)
			{
				stream << " default_server";
			}
			stream << ";" << std::endl;
		}
		const std::set<std::string> &serverNames =
			printableServerConfig.getServerNames();
		if (!serverNames.empty())
		{
			stream << indent << indent << "server_name";
			for (std::set<std::string>::const_iterator nameIterator =
			         serverNames.begin();
			     nameIterator != serverNames.end(); ++nameIterator)
			{
				stream << " " << *nameIterator;
			}
			stream << ";" << std::endl;
		}
		serverConfig->printTo(stream, depth + 1);
		stream << indent << "}" << std::endl;
	}

	if (limitExcept.checkIfSet())
	{
		const LimitExceptValue &limitExceptValue = limitExcept.get();
		stream << indent << "limit_except";
		const std::set<HttpMethod> &exemptMethods =
			limitExceptValue.getExemptMethods();
		for (std::set<HttpMethod>::const_iterator iterator =
		         exemptMethods.begin();
		     iterator != exemptMethods.end(); ++iterator)
		{
			stream << " "
				   << HttpMethodHelpers::getStringForHttpMethod(*iterator);
		}
		stream << " {" << std::endl;
		limitExceptValue.getLimitExceptConfig().printTo(stream, depth + 1);
		stream << indent << "}" << std::endl;
	}

	for (std::map<std::string, LocationConfig *>::const_iterator iterator =
	         exactLocationConfigs.begin();
	     iterator != exactLocationConfigs.end(); ++iterator)
	{
		stream << indent << "location = " << iterator->first << " {"
			   << std::endl;
		iterator->second->printTo(stream, depth + 1);
		stream << indent << "}" << std::endl;
	}

	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         priorityPrefixLocationConfigs.begin();
	     iterator != priorityPrefixLocationConfigs.end(); ++iterator)
	{
		stream << indent << "location ^~ " << iterator->first << " {"
			   << std::endl;
		iterator->second->printTo(stream, depth + 1);
		stream << indent << "}" << std::endl;
	}

	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         prefixLocationConfigs.begin();
	     iterator != prefixLocationConfigs.end(); ++iterator)
	{
		stream << indent << "location " << iterator->first << " {" << std::endl;
		iterator->second->printTo(stream, depth + 1);
		stream << indent << "}" << std::endl;
	}

	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         suffixLocationConfigs.begin();
	     iterator != suffixLocationConfigs.end(); ++iterator)
	{
		stream << indent << "location ~$ " << iterator->first << " {"
			   << std::endl;
		iterator->second->printTo(stream, depth + 1);
		stream << indent << "}" << std::endl;
	}
}

std::ostream &operator<<(std::ostream &stream, const AConfig &config)
{
	config.printTo(stream);
	return stream;
}
