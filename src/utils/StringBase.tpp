/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringBase.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:29:08 by emflynn           #+#    #+#             */
/*   Updated: 2026/04/19 23:32:03 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if !defined(STRING_BASE_TPP_WRAPPER) && !defined(IGNORE_TPP_WRAPPERS)
# error "StringBase.tpp should be included via StringBase.hpp, not directly"
#endif

#ifndef STRING_BASE_TPP
# define STRING_BASE_TPP

# include "StringBase.hpp"

template<typename TElement>
StringBase &StringBase::operator<<(const TElement &element)
{
	stream << element;
	return *this;
}

#endif
