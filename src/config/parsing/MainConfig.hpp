/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 02:42:32 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/03 16:44:50 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_CONFIG_HPP
#define MAIN_CONFIG_HPP

#include <string>

#include "AConfig.hpp"

class MainConfig: public AConfig
{
public:
	MainConfig(void);
	MainConfig(const MainConfig &other);
	MainConfig &operator=(const MainConfig &other);
	~MainConfig(void);

	void freeze(const std::string &configFilePath);

protected:
	void throwIfFrozen(void) const;
	ConfigType getConfigType(void) const;

private:
	bool isFrozen;
};

#endif
