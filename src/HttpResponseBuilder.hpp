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

#ifndef HTTPRESPONSEBUILDER_HPP
# define HTTPRESPONSEBUILDER_HPP

# include "HttpResponse.hpp"
# include <sstream>

class	HttpResponseBuilder
{
	public:
		static std::string	build(const HttpResponse &res)
		{
			std::ostringstream	out;
			out << res.version << " " << res.statusCode << " " << res.description << "\r\n";
			std::map<std::string, std::string>::const_iterator	it;
			for (it = res.headers.begin(); it != res.headers.end(); ++it)
			{
				out << it->first << ": " << it->second << "\r\n";
			}
			out << "Content-Length: " << res.body.size() << "\r\n";
			out << "\r\n";
			out << res.body;
			return (out.str());
		}
};

# endif
