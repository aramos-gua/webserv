/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OverridableConfigSetting.tpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:29:08 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/01 04:58:41 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if !defined(OVERRIDABLE_CONFIG_SETTING_TPP_WRAPPER) &&                        \
	!defined(IGNORE_TPP_WRAPPERS)
# error                                                                         \
	 "OverridableConfigSetting.tpp should be included via OverridableConfigSetting.hpp, not directly"
#endif

#ifndef OVERRIDABLE_CONFIG_SETTING_TPP
# define OVERRIDABLE_CONFIG_SETTING_TPP

# include "OverridableConfigSetting.hpp"

template<typename TSetting>
OverridableConfigSetting<TSetting>::OverridableConfigSetting(void)
	: isSet(false), setting()
{
}

template<typename TSetting>
OverridableConfigSetting<TSetting>::OverridableConfigSetting(
	const OverridableConfigSetting &other)
	: isSet(other.isSet), setting(other.setting)
{
}

template<typename TSetting>
OverridableConfigSetting<TSetting> &OverridableConfigSetting<
	TSetting>::operator=(const OverridableConfigSetting &other)
{
	if (this == &other)
	{
		return *this;
	}
	isSet = other.isSet;
	setting = other.setting;
	return *this;
}

template<typename TSetting>
OverridableConfigSetting<TSetting>::~OverridableConfigSetting(void)
{
}

template<typename TSetting>
bool OverridableConfigSetting<TSetting>::checkIfSet(void) const
{
	return isSet;
}

template<typename TSetting>
const TSetting &OverridableConfigSetting<TSetting>::get(void) const
{
	return setting;
}

template<typename TSetting>
TSetting &OverridableConfigSetting<TSetting>::get(void)
{
	return setting;
}

template<typename TSetting>
void OverridableConfigSetting<TSetting>::set(const TSetting &setting)
{
	this->setting = setting;
	isSet = true;
}

#endif
