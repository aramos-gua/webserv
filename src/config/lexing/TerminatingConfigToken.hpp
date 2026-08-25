/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TerminatingConfigToken.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:38:09 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/14 16:56:09 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINATING_CONFIG_TOKEN_HPP
#define TERMINATING_CONFIG_TOKEN_HPP

#include <cstddef>

#include "AConfigToken.hpp"

class TerminatingConfigToken: public AConfigToken
{
public:
	TerminatingConfigToken(void);
	TerminatingConfigToken(std::size_t lineNumber);
	TerminatingConfigToken(const TerminatingConfigToken &other);
	TerminatingConfigToken &operator=(const TerminatingConfigToken &other);
	~TerminatingConfigToken(void);
};

#endif
