/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OverridableConfigSetting.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 03:16:06 by emflynn           #+#    #+#             */
/*   Updated: 2026/06/01 04:58:04 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OVERRIDABLE_CONFIG_SETTING_HPP
#define OVERRIDABLE_CONFIG_SETTING_HPP

template<typename TSetting>
class OverridableConfigSetting
{
public:
	OverridableConfigSetting(void);
	OverridableConfigSetting(const OverridableConfigSetting &other);
	OverridableConfigSetting &operator=(const OverridableConfigSetting &other);
	~OverridableConfigSetting(void);

	bool checkIfSet(void) const;

	const TSetting &get(void) const;
	TSetting &get(void);
	void set(const TSetting &setting);

private:
	bool isSet;

	TSetting setting;
};

#define OVERRIDABLE_CONFIG_SETTING_TPP_WRAPPER
#include "OverridableConfigSetting.tpp"
#undef OVERRIDABLE_CONFIG_SETTING_TPP_WRAPPER

#endif
