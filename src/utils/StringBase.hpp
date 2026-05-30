/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringBase.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:23:55 by emflynn           #+#    #+#             */
/*   Updated: 2026/05/24 17:09:27 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_BASE_HPP
#define STRING_BASE_HPP

#include <ostream>
#include <sstream>
#include <string>

class StringBase
{
public:
	StringBase(void);
	StringBase(const StringBase &other);
	StringBase &operator=(const StringBase &other);
	~StringBase(void);

	template<typename TElement>
	StringBase &operator<<(const TElement &element);
	StringBase &operator<<(std::ostream &(*manipulator)(std::ostream &));

	operator std::string(void) const;

private:
	std::ostringstream stream;
};

#define STRING_BASE_TPP_WRAPPER
#include "StringBase.tpp"
#undef STRING_BASE_TPP_WRAPPER

#endif
