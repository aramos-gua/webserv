/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringBase.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:32:28 by emflynn           #+#    #+#             */
/*   Updated: 2026/04/22 18:27:33 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StringBase.hpp"

StringBase::StringBase(void)
{
}

StringBase::StringBase(const StringBase &other): stream(other.stream.str())
{
}

StringBase &StringBase::operator=(const StringBase &other)
{
	if (this == &other)
	{
		return *this;
	}
	stream.str(other.stream.str());
	return *this;
}

StringBase::~StringBase(void)
{
}

StringBase &StringBase::operator<<(std::ostream &(*manipulator)(std::ostream &))
{
	manipulator(stream);
	return *this;
}

StringBase::operator std::string() const
{
	return stream.str();
}
