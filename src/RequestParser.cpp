/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aramos <contact@aramos.dev>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:35:42 by aramos            #+#    #+#             */
/*   Updated: 2026/05/30 12:57:58 by aramos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"
#include <stdexcept>
#include <sstream>
/* ************************************************************************** */
/*                              CONSTRUCTOR                                   */
/* ************************************************************************** */
RequestParser::RequestParser(size_t maxBodySize) : 
	_state(PARSE_REQUEST_LINE),
	_buffer(),
	_req(),
	_error(),
	_maxBodySize(maxBodySize),
	_bodyBytesNeeded(0)
{}

/* ************************************************************************** */
/*                              COPY CONSTRUCTOR                              */
/* ************************************************************************** */
RequestParser::RequestParser(const RequestParser &other) :
	_state(other.state),
	_buffer(other.buffer),
	_req(other.req),
	_error(other.error),
	_maxBodySize(other.maxBodySize),
	_bodyBytesNeeded(other._bodyBytesNeeded)
{}

/* ************************************************************************** */
/*                          COPY ASSIGNMENT OPERATOR                          */
/* ************************************************************************** */
RequestParser &RequestParser::operator=(const RequestParser &other)
{
	if (this != other)
	{
		_state = other._state;
		_buffer = other._buffer;
		_req = other._req;
		_error = other._error;
		_maxBodySize = other._maxBodySize;
		_bodyBytesNeeded = other._bodyBytesNeeded;
	}
	return (*this);
}

/* ************************************************************************** */
/*                               DESTRUCTOR                                   */
/* ************************************************************************** */
RequestParser::~RequestParser()
{
	return ;
}

/* ************************************************************************** */
/*                               MEMBER FUNCTIONS                             */
/* ************************************************************************** */
static std::string	trim(const std::string& str)
{
	size_t	start;
	size_t	end;

	start = 0;
	while (start < str.size() && (str[start] == ' ' || str[start] == '\t' || str[start] == '\r'))
		start++;
	end = str.size();
	while (end > start && (str[end - 1] == ' ' || str[end -1] == '\t' || str[end - 1] == '\r'))
		end--;
	return (str.substr(start, end - start));
}

void	RequestParser::reset()
{
	_state = PARSE_REQUEST_LINE;
	_buffer.clear();
	_req = HttpRequest();
	_error.clear();
	_bodyBytesNeeded = 0;
}

const	HttpRequest &RequestParser::getRequest() const
{
	return (_req);
}

const std::string &RequestParser::getError() const
{
	return (_error);
}

bool	RequestParser::parseRequestLine()
{
	size_t	pos = _buffer.find("\r\n");
	if (pos == std::string::npos)
		return (false);
}

RequestParser::Result RequestParser::feed(const char *data, size_t len)
{
	if (_state == PARSE_ERROR)
		return (ERROR);
	if (_state == PARSE_DONE)
		return (COMPLETE);
	_buffer.append(data, len);
	bool progress = true;
	while (progress)
	{
		progress = false;
		if (_state == PARSE_REQUEST_LINE)
			progress = parseRequestLine();
		else if (_state == PARSE_HEADERS)
			progress = parseHeaders();
		else if (_state == PARSE_BODY)
			progress = parseBody();
		else if (_state == PARSE_DONE)
			return (COMPLETE);
		else if (_state == PARSE_ERROR)
			return (ERROR);
	}
	if (_state == PARSE_DONE)
		return (COMPLETE);
	if (_state == PARSE_ERROR)
		return (ERROR);
	return (INCOMPLETE);
}
/* ************************************************************************** */
/*                                OTHER FUNCTIONS                             */
/* ************************************************************************** */
HttpRequest	RequestParser::parse(const std::string& rawRequest)
{
	HttpRequest	req;
	
	std::istringstream	stream(rawRequest);
	std::string			line;

	if (!std::getline(stream, line))
		throw std::runtime_error("Empty request");
	std::istringstream	requestLine(line);
	if(!(requestLine >> req.method >> req.path >> req.version))
		throw(std::runtime_error("Invalid request line"));
	while (std::getline(stream, line))
	{
		if (line == "\r" || line.empty())
			break;
		size_t	separator = line.find(":");
		if (separator == std::string::npos)
			continue;
		std::string	key = trim(line.substr(0, separator));
		std::string	value = trim(line.substr(separator + 1));
		req.headers[key] = value;
	}
	std::string	body;
	while (std::getline(stream, line))
	{
		body += line;
		if (!stream.eof())
			body += "\n";
	}
	req.body = body;
	return req;
}
