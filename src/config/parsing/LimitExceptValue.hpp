/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptValue.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 17:25:37 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:55:28 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMIT_EXCEPT_VALUE
#define LIMIT_EXCEPT_VALUE

#include <set>

#include "HttpMethod.hpp"

class LimitExceptConfig;

class LimitExceptValue
{
public:
	LimitExceptValue(void);
	LimitExceptValue(const std::set<HttpMethod> &exemptMethods,
	                 const LimitExceptConfig &limitExceptConfig);
	LimitExceptValue(const LimitExceptValue &other);
	LimitExceptValue &operator=(const LimitExceptValue &other);
	~LimitExceptValue(void);

	const std::set<HttpMethod> &getExemptMethods(void) const;
	const LimitExceptConfig &getLimitExceptConfig(void) const;

private:
	std::set<HttpMethod> exemptMethods;
	LimitExceptConfig *limitExceptConfig;
};

#endif
