/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 02:54:58 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/25 06:39:28 by emflynn          ###   ########.fr       */
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
#include "PrintableServerConfig.hpp"
#include "ServerConfig.hpp"
#include "ServerNameHelpers.hpp"
#include "SpecialServerNames.hpp"
#include "StringBase.hpp"

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

AConfig *AConfig::getParentConfig(void)
{
	return parentConfig;
}

void AConfig::setParentConfig(AConfig *parentConfig)
{
	this->parentConfig = parentConfig;
}

bool AConfig::getWhetherShouldRunAsDaemonSet(void) const
{
	return shouldRunAsDaemon.checkIfSet();
}

bool AConfig::getWhetherShouldRunAsDaemon(void) const
{
	return shouldRunAsDaemon.get();
}

void AConfig::setWhetherShouldRunAsDaemon(bool shouldRunAsDaemon)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherShouldRunAsDaemonSet());
	this->shouldRunAsDaemon.set(shouldRunAsDaemon);
}

bool AConfig::getWhetherErrorLogFilePathSet(void) const
{
	return errorLogFilePath.checkIfSet();
}

const std::string &AConfig::getErrorLogFilePath(void) const
{
	return errorLogFilePath.get();
}

void AConfig::setErrorLogFilePath(const std::string &errorLogFilePath)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN, HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherErrorLogFilePathSet());
	this->errorLogFilePath.set(errorLogFilePath);
}

bool AConfig::getWhetherEventsConfigSet(void) const
{
	return eventsConfig != NULL;
}

const EventsConfig &AConfig::getEventsConfig(void) const
{
	return *eventsConfig;
}

void AConfig::setEventsConfig(const EventsConfig &eventsConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherEventsConfigSet());
	this->eventsConfig = new EventsConfig(eventsConfig);
	this->eventsConfig->setParentConfig(this);
}

bool AConfig::getWhetherWorkerUserSet(void) const
{
	return workerUser.checkIfSet();
}

const std::string &AConfig::getWorkerUser(void) const
{
	return workerUser.get();
}

void AConfig::setWorkerUser(const std::string &workerUser)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherWorkerUserSet());
	this->workerUser.set(workerUser);
}

bool AConfig::getWhetherWorkerGroupSet(void) const
{
	return workerGroup.checkIfSet();
}

const std::string &AConfig::getWorkerGroup(void) const
{
	return workerGroup.get();
}

void AConfig::setWorkerGroup(const std::string &workerGroup)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherWorkerGroupSet());
	this->workerGroup.set(workerGroup);
}

bool AConfig::getWhetherWorkerMaxConnectionsSet(void) const
{
	return workerMaxConnections.checkIfSet();
}

std::size_t AConfig::getWorkerMaxConnections(void) const
{
	return workerMaxConnections.get();
}

void AConfig::setWorkerMaxConnections(std::size_t workerMaxConnections)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {EVENTS};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherWorkerMaxConnectionsSet());
	this->workerMaxConnections.set(workerMaxConnections);
}

bool AConfig::getWhetherWorkerProcessesSet(void) const
{
	return workerProcesses.checkIfSet();
}

std::size_t AConfig::getWorkerProcesses(void) const
{
	return workerProcesses.get();
}

void AConfig::setWorkerProcesses(std::size_t workerProcesses)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherWorkerProcessesSet());
	this->workerProcesses.set(workerProcesses);
}

bool AConfig::getWhetherAliasSet(void) const
{
	return alias.checkIfSet();
}

const std::string &AConfig::getAlias(void) const
{
	return alias.get();
}

void AConfig::setAlias(const std::string &alias)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherAliasSet());
	this->alias.set(alias);
}

bool AConfig::getWhetherClientMaxBodySizeSet(void) const
{
	return clientMaxBodySize.checkIfSet();
}

std::size_t AConfig::getClientMaxBodySize(void) const
{
	return clientMaxBodySize.get();
}

void AConfig::setClientMaxBodySize(std::size_t clientMaxBodySize)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherClientMaxBodySizeSet());
	this->clientMaxBodySize.set(clientMaxBodySize);
}

bool AConfig::getWhetherDefaultMimeTypeSet(void) const
{
	return defaultMimeType.checkIfSet();
}

const std::string &AConfig::getDefaultMimeType(void) const
{
	return defaultMimeType.get();
}

void AConfig::setDefaultMimeType(const std::string &defaultMimeType)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherDefaultMimeTypeSet());
	this->defaultMimeType.set(defaultMimeType);
}

const ErrorPageValue &AConfig::getErrorPageForHttpStatusCode(
	HttpStatusCode httpStatusCode) const
{
	return errorPages.at(httpStatusCode);
}

void AConfig::setErrorPageForHttpStatusCode(
	HttpStatusCode httpStatusCode, const ErrorPageValue &errorPageValue)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(errorPages.find(httpStatusCode) != errorPages.end());
	errorPages[httpStatusCode] = errorPageValue;
}

bool AConfig::getWhetherHttpConfigSet(void) const
{
	return httpConfig != NULL;
}

const HttpConfig &AConfig::getHttpConfig(void) const
{
	return *httpConfig;
}

void AConfig::setHttpConfig(const HttpConfig &httpConfig)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {MAIN};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherHttpConfigSet());
	this->httpConfig = new HttpConfig(httpConfig);
	this->httpConfig->setParentConfig(this);
}

bool AConfig::getWhetherIsInternalSet(void) const
{
	return isInternal.checkIfSet();
}

bool AConfig::getWhetherIsInternal(void) const
{
	return isInternal.get();
}

void AConfig::setWhetherIsInternal(bool isInternal)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherIsInternalSet());
	this->isInternal.set(isInternal);
}

bool AConfig::getWhetherLimitExceptSet(void) const
{
	return limitExcept.checkIfSet();
}

const LimitExceptConfig *AConfig::getLimitExceptConfigForMethod(
	HttpMethod method) const
{
	if (!getWhetherLimitExceptSet())
	{
		return NULL;
	}
	const std::set<HttpMethod> &exemptMethods =
		limitExcept.get().getExemptMethods();
	if (exemptMethods.find(method) != exemptMethods.end())
	{
		return NULL;
	}
	return &limitExcept.get().getLimitExceptConfig();
}

void AConfig::setLimitExcept(const LimitExceptValue &limitExcept)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherLimitExceptSet());
	this->limitExcept.set(limitExcept);
}

const LocationConfig &AConfig::getExactLocationConfigForPath(
	const std::string &path) const
{
	return *exactLocationConfigs.at(path);
}

void AConfig::setLocationConfigForExactPath(
	const std::string &exactPath, const LocationConfig &locationConfig)
{
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(exactLocationConfigs.find(exactPath) !=
	                  exactLocationConfigs.end());
	exactLocationConfigs[exactPath] = new LocationConfig(locationConfig);
	exactLocationConfigs[exactPath]->setParentConfig(this);
}

const LocationConfig &AConfig::getPrefixLocationConfigForPath(
	const std::string &path) const
{
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         prefixLocationConfigs.lower_bound(path);
	     iterator != prefixLocationConfigs.end(); ++iterator)
	{
		if (!path.compare(0, iterator->first.size(), iterator->first))
		{
			return *iterator->second;
		}
	}
	throw std::out_of_range(StringBase()
	                        << "No prefix match found for path " << path);
}

void AConfig::setLocationConfigForPathPrefix(
	const std::string &pathPrefix, const LocationConfig &locationConfig)
{
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::iterator iterator =
	         prefixLocationConfigs.lower_bound(pathPrefix);
	     iterator != prefixLocationConfigs.upper_bound(pathPrefix); ++iterator)
	{
		if (pathPrefix == iterator->first)
		{
			throwIfAlreadySet(true);
		}
	}
	prefixLocationConfigs
		.insert(std::pair<std::string, LocationConfig *>(
			pathPrefix, new LocationConfig(locationConfig)))
		->second->setParentConfig(this);
}

const LocationConfig &AConfig::getSuffixLocationConfigForPath(
	const std::string &path) const
{
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::const_iterator iterator =
	         suffixLocationConfigs.lower_bound(path);
	     iterator != suffixLocationConfigs.end(); ++iterator)
	{
		if (!path.compare(path.size() - iterator->first.size(), path.size(),
		                  iterator->first))
		{
			return *iterator->second;
		}
	}
	throw std::out_of_range(StringBase()
	                        << "No suffix match found for path " << path);
}

void AConfig::setLocationConfigForPathSuffix(
	const std::string &pathSuffix, const LocationConfig &locationConfig)
{
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	for (std::multimap<std::string, LocationConfig *,
	                   LongestStringFirstComparator>::iterator iterator =
	         suffixLocationConfigs.lower_bound(pathSuffix);
	     iterator != suffixLocationConfigs.upper_bound(pathSuffix); ++iterator)
	{
		if (pathSuffix == iterator->first)
		{
			throwIfAlreadySet(true);
		}
	}
	suffixLocationConfigs
		.insert(std::pair<std::string, LocationConfig *>(
			pathSuffix, new LocationConfig(locationConfig)))
		->second->setParentConfig(this);
}

bool AConfig::getWhetherRootSet(void) const
{
	return root.checkIfSet();
}

const std::string &AConfig::getRoot(void) const
{
	return root.get();
}

void AConfig::setRoot(const std::string &root)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherRootSet());
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
		throwIfAlreadySet(
			pendingListenAddressPortPairs.find(canonicalPairs[i]) !=
			pendingListenAddressPortPairs.end());
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
	throwIfAlreadySet(pendingServerNames.checkIfSet());
	pendingServerNames.set(serverNames);
}

void AConfig::registerAsServerIn(AConfig &parentConfig)
{
	static const ConfigType ALLOWED_TYPES[] = {SERVER};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	parentConfig.setServerConfig(*dynamic_cast<ServerConfig *>(this));
}

bool AConfig::getWhetherAnyServerConfigsSet(void) const
{
	return !serverConfigs.empty();
}

std::vector<std::string> AConfig::getServerConfigAddressPortPairs(void) const
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

const ServerConfig &AConfig::getServerConfig(
	const std::string &addressPortPair, const std::string &serverName) const
{
	const t_server_configs_by_server_name &serverConfigsForServerNames =
		serverConfigs.at(addressPortPair);
	try
	{
		return *serverConfigsForServerNames.at(serverName);
	}
	catch (const std::out_of_range &)
	{
		try
		{
			return *serverConfigsForServerNames.at(SpecialServerNames::DEFAULT);
		}
		catch (const std::out_of_range &)
		{
			return *serverConfigsForServerNames.at(SpecialServerNames::FIRST);
		}
	}
}

void AConfig::setServerConfig(const ServerConfig &serverConfig)
{
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

bool AConfig::getWhetherTryFilesSet(void) const
{
	return tryFiles.checkIfSet();
}

const TryFilesValue &AConfig::getTryFiles(void) const
{
	return tryFiles.get();
}

void AConfig::setTryFiles(const TryFilesValue &tryFiles)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherTryFilesSet());
	this->tryFiles.set(tryFiles);
}

bool AConfig::getWhetherMimeTypesForExtensionsSet(void) const
{
	return mimeTypesForExtensions.checkIfSet();
}

const std::map<std::string, std::string> &AConfig::getMimeTypesForExtensions(
	void) const
{
	return mimeTypesForExtensions.get();
}

void AConfig::setMimeTypesForExtensions(
	const std::map<std::string, std::string> &mimeTypesForExtensions)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherMimeTypesForExtensionsSet());
	this->mimeTypesForExtensions.set(mimeTypesForExtensions);
}

AccessRule AConfig::getAccessRuleForIpAddress(
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
		return parentConfig->getAccessRuleForIpAddress(ipAddress);
	}
	return ALLOW;
}

void AConfig::setAccessRuleForIpAddressAndMask(
	const std::string &ipAddressAndMask, AccessRule accessRule)
{
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
			throwIfAlreadySet(true);
		}
	}
	accessRules.insert(
		std::pair<std::string, AccessRule>(canonicalKey, accessRule));
}

bool AConfig::getWhetherShouldUseAutoindexSet(void) const
{
	return shouldUseAutoindex.checkIfSet();
}

bool AConfig::getWhetherShouldUseAutoindex(void) const
{
	return shouldUseAutoindex.get();
}

void AConfig::setWhetherShouldUseAutoindex(bool shouldUseAutoindex)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherShouldUseAutoindexSet());
	this->shouldUseAutoindex.set(shouldUseAutoindex);
}

bool AConfig::getWhetherIndexesSet(void) const
{
	return indexes.checkIfSet();
}

const std::vector<std::string> &AConfig::getIndexes(void) const
{
	return indexes.get();
}

void AConfig::setIndexes(const std::vector<std::string> &indexes)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherIndexesSet());
	this->indexes.set(indexes);
}

bool AConfig::getWhetherAccessLogFilePathSet(void) const
{
	return accessLogFilePath.checkIfSet();
}

const std::string &AConfig::getAccessLogFilePath(void) const
{
	return accessLogFilePath.get();
}

void AConfig::setAccessLogFilePath(const std::string &accessLogFilePath)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {HTTP, SERVER, LOCATION,
	                                           LIMIT_EXCEPT};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherAccessLogFilePathSet());
	this->accessLogFilePath.set(accessLogFilePath);
}

bool AConfig::getWhetherReturnResponseSet(void) const
{
	return returnResponse.checkIfSet();
}

const ReturnResponseValue &AConfig::getReturnResponse(void) const
{
	return returnResponse.get();
}

void AConfig::setReturnResponse(const ReturnResponseValue &returnResponse)
{
	throwIfFrozen();
	static const ConfigType ALLOWED_TYPES[] = {SERVER, LOCATION};
	throwIfNotSupportedForConfigType(ALLOWED_TYPES, sizeof(ALLOWED_TYPES) /
	                                                    sizeof(ConfigType));
	throwIfAlreadySet(getWhetherReturnResponseSet());
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

void AConfig::setUpFrom(const AConfig &other)
{
	shouldRunAsDaemon = other.shouldRunAsDaemon;
	errorLogFilePath = other.errorLogFilePath;
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
	if (serverConfigs.find(addressPortPair) == serverConfigs.end())
	{
		serverConfigs[addressPortPair] = t_server_configs_by_server_name();
		serverConfigs.at(addressPortPair)[SpecialServerNames::FIRST] =
			serverConfig;
	}
	t_server_configs_by_server_name &serverConfigsForAddressPortPair =
		serverConfigs.at(addressPortPair);
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
		const std::string &serverName =
			ServerNameHelpers::isValidServerName(*serverNameIterator)
				? *serverNameIterator
				: SpecialServerNames::CATCH_ALL;
		if (serverConfigsForAddressPortPair.find(serverName) !=
		    serverConfigsForAddressPortPair.end())
		{
			throw std::runtime_error(
				StringBase()
				<< "More than one server definition provided for "
				<< addressPortPair << " for server with "
				<< (serverName == SpecialServerNames::CATCH_ALL
			            ? "fallback name"
			            : ("name " +
			               (serverName.empty() ? "\"\"" : serverName))));
		}
		serverConfigsForAddressPortPair[serverName] = serverConfig;
	}
}

AConfig::ConfigFrozenException::ConfigFrozenException(void)
	: std::logic_error("Cannot modify config once it has been frozen")
{
}

AConfig::DirectiveAlreadySetException::DirectiveAlreadySetException(void)
	: std::runtime_error("Directive already set for this context")
{
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

	if (errorLogFilePath.checkIfSet())
	{
		stream << indent << "error_log " << errorLogFilePath.get() << ";"
			   << std::endl;
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
