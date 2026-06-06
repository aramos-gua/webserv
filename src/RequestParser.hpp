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
# include <string>

class	RequestParser
{
	public:
		enum	Result {
					INCOMPLETE,
					COMPLETE,
					ERROR
		};

		RequestParser(size_t maxBodySize = 1024 * 1024);
		RequestParser(const RequestParser other);
		RequestParser &operator=(const RequestParser &other);
		~RequestParser();

		Result	feed(const char *data, size_t len);
		const	HttpRequest	&getRequest() const;
		const	std::string	&getError() const;
		void	reset();

	private:
		enum	State {
				PARSE_REQUEST_LINE;
				PARSE_HEADERS;
				PARSE_BODY;
				PARSE_DONE;
				PARSE_ERROR;
		};

		State		_state;
		std::string	_buffer;
		HttpRequest	_req;
		std::string	_error;

		size_t				_maxBodySize;
		size_t				_bodyBytesNeeded;

		bool				parseRequestLine();
		bool				parseHeaders();
		bool				parseBody();

		static std::string	trim(const std::string &s);
};

#endif
