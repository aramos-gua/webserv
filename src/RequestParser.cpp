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

#include <cctype>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <stdexcept>

#include "RequestParser.hpp"

/* ************************************************************************** */
/*                              CONSTRUCTOR                                   */
/* ************************************************************************** */
RequestParser::RequestParser(size_t maxBodySize)
	: _state(PARSE_REQUEST_LINE), _buffer(), _req(), _error(),
	  _maxBodySize(maxBodySize), _bodyBytesNeeded(0), _headerCount(0)
{
}

/* ************************************************************************** */
/*                              COPY CONSTRUCTOR                              */
/* ************************************************************************** */
RequestParser::RequestParser(const RequestParser &other)
	: _state(other._state), _buffer(other._buffer), _req(other._req),
	  _error(other._error), _maxBodySize(other._maxBodySize),
	  _bodyBytesNeeded(other._bodyBytesNeeded), _headerCount(other._headerCount)
{
}

/* ************************************************************************** */
/*                          COPY ASSIGNMENT OPERATOR                          */
/* ************************************************************************** */
RequestParser &RequestParser::operator=(const RequestParser &other)
{
	if (this != &other)
	{
		_state = other._state;
		_buffer = other._buffer;
		_req = other._req;
		_error = other._error;
		_maxBodySize = other._maxBodySize;
		_bodyBytesNeeded = other._bodyBytesNeeded;
		_headerCount = other._headerCount;
	}
	return *this;
}

/* ************************************************************************** */
/*                               DESTRUCTOR                                   */
/* ************************************************************************** */
RequestParser::~RequestParser()
{
	return;
}

/* ************************************************************************** */
/*                               MEMBER FUNCTIONS                             */
/* ************************************************************************** */
std::string RequestParser::trim(const std::string &str)
{
	size_t start;
	size_t end;

	start = 0;
	while (start < str.size() &&
	       (str[start] == ' ' || str[start] == '\t' || str[start] == '\r'))
	{
		start++;
	}
	end = str.size();
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t' ||
	                       str[end - 1] == '\r'))
	{
		end--;
	}
	return str.substr(start, end - start);
}

bool RequestParser::isValidContentLength(const std::string &s, size_t &out)
{
	if (s.empty())
	{
		return false;
	}
	size_t val = 0;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (!isdigit(static_cast<unsigned char>(s[i])))
		{
			return false;
		}
		size_t digit = s[i] - '0';
		if (val > (std::numeric_limits<size_t>::max() - digit) / 10)
		{
			return false;
		}
		val = val * 10 + digit;
	}
	out = val;
	return true;
}

void RequestParser::reset()
{
	_state = PARSE_REQUEST_LINE;
	_buffer.clear();
	_req = HttpRequest();
	_error.clear();
	_bodyBytesNeeded = 0;
	_headerCount = 0;
}

const HttpRequest &RequestParser::getRequest() const
{
	return _req;
}

const std::string &RequestParser::getError() const
{
	return _error;
}

bool RequestParser::parseRequestLine()
{
	static const size_t MAX_REQUEST_LINE = 8192;
	size_t pos = _buffer.find("\r\n");
	if (pos == std::string::npos)
	{
		if (_buffer.size() > MAX_REQUEST_LINE)
		{
			_state = PARSE_ERROR;
			_error = "Request line too large";
		}
		return false;
	}
	std::string line = _buffer.substr(0, pos);
	_buffer.erase(0, pos + 2);
	std::istringstream iss(line);
	if (!(iss >> _req.method >> _req.path >> _req.version))
	{
		_state = PARSE_ERROR;
		_error = "Invalid request line";
		return false;
	}
	_state = PARSE_HEADERS;
	return true;
}

bool RequestParser::parseHeaders()
{
	static const size_t MAX_HEADERS_LINE = 8192;
	size_t pos = _buffer.find("\r\n");
	if (pos == std::string::npos)
	{
		if (_buffer.size() > MAX_HEADERS_LINE)
		{
			_state = PARSE_ERROR;
			_error = "Headers too large";
		}
		return false;
	}
	std::string line = _buffer.substr(0, pos);
	_buffer.erase(0, pos + 2);
	if (line.empty())
	{
		if (_req.headers.count("content-length"))
		{
			if (!isValidContentLength(_req.headers["content-length"],
			                          _bodyBytesNeeded))
			{
				_state = PARSE_ERROR;
				_error = "Invalid Content-Length";
				return false;
			}
			if (_bodyBytesNeeded > _maxBodySize)
			{
				_state = PARSE_ERROR;
				_error = "Body too large";
				return false;
			}
		}
		_state = PARSE_BODY;
		return true;
	}
	size_t sep = line.find(":");
	if (sep == std::string::npos)
	{
		_state = PARSE_ERROR;
		_error = "Malformed header line";
		return false;
	}
	static const size_t MAX_HEADER_COUNT = 100;
	if (++_headerCount > MAX_HEADER_COUNT)
	{
		_state = PARSE_ERROR;
		_error = "Too many headers";
		return false;
	}
	std::string key = trim(line.substr(0, sep));
	for (size_t i = 0; i < key.size(); ++i)
	{
		key[i] = tolower(static_cast<unsigned char>(key[i]));
	}
	std::string val = trim(line.substr(sep + 1));
	if (key == "content-length" && _req.headers.count(key))
	{
		_state = PARSE_ERROR;
		_error = "Duplicate Content-Length";
		return false;
	}
	_req.headers[key] = val;
	return true;
}

bool RequestParser::parseBody()
{
	if (_bodyBytesNeeded == 0)
	{
		_state = PARSE_DONE;
		return true;
	}
	if (_buffer.size() < _bodyBytesNeeded)
	{
		return false;
	}
	_req.body = _buffer.substr(0, _bodyBytesNeeded);
	_req.contentLength = _bodyBytesNeeded;
	_buffer.erase(0, _bodyBytesNeeded);
	_state = PARSE_DONE;
	return true;
}

RequestParser::Result RequestParser::feed(const char *data, size_t len)
{
	if (_state == PARSE_ERROR)
	{
		return ERROR;
	}
	if (_state == PARSE_DONE)
	{
		return COMPLETE;
	}
	_buffer.append(data, len);
	bool progress = true;
	while (progress)
	{
		progress = false;
		if (_state == PARSE_REQUEST_LINE)
		{
			progress = parseRequestLine();
		}
		else if (_state == PARSE_HEADERS)
		{
			progress = parseHeaders();
		}
		else if (_state == PARSE_BODY)
		{
			progress = parseBody();
		}
	}
	if (_state == PARSE_DONE)
	{
		return COMPLETE;
	}
	if (_state == PARSE_ERROR)
	{
		return ERROR;
	}
	return INCOMPLETE;
}
