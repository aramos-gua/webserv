/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manwar <manwar@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:40:14 by manwar            #+#    #+#             */
/*   Updated: 2026/08/25 21:40:15 by manwar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>

// TODO: Replace with the real parsed config
struct ServerConfig
{
	ServerConfig();
	ServerConfig(const ServerConfig &copy);
	ServerConfig &operator=(const ServerConfig &copy);
	~ServerConfig();

	int port;
	std::string host;
};

#endif
