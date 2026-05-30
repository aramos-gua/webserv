/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aramos <contact@aramos.dev>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:33:06 by aramos            #+#    #+#             */
/*   Updated: 2026/05/30 12:35:24 by aramos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include "HttpRequest.hpp"

class	RequestParser
{
	public:
		static	HttpRequest	parse(const std::string& rawRequest);
};

#endif
