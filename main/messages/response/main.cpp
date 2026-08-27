/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:10:00 by emflynn           #+#    #+#             */
/*   Updated: 2026/08/27 19:10:00 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "HttpResponseBuilder.hpp"
#include "HttpVersionHelpers.hpp"

// The builder produces bytes rather than consuming them, so a response file
// describes the response to build instead of holding a message to parse:
//
//   STATUS 204
//   VERSION HTTP/1.1
//   HEADER Content-Type: text/plain
//   BODY
//   everything from here to the end of the file, verbatim
//
// Every directive is optional and BODY must come last. One trailing newline is
// stripped from the body, so that a file can end the way an editor leaves it.
static const char *const USAGE_SUFFIX = " [--raw] <path to response file>";

static std::string readWholeFile(const std::string &filePath)
{
	std::ifstream fileStream(filePath.c_str(), std::ios::binary);

	if (!fileStream.is_open())
	{
		throw std::runtime_error("Couldn't open response file \"" + filePath +
		                         "\" for reading");
	}
	std::ostringstream contents;
	contents << fileStream.rdbuf();
	return contents.str();
}

static std::string takeLine(const std::string &text, std::size_t &offset)
{
	std::size_t lineEnd = text.find('\n', offset);

	if (lineEnd == std::string::npos)
	{
		std::string line = text.substr(offset);
		offset = text.size();
		return line;
	}
	std::string line = text.substr(offset, lineEnd - offset);
	offset = lineEnd + 1;
	return line;
}

// Header values are the one place a test needs to smuggle a real CR or LF into
// the builder, which a line-oriented spec file cannot hold literally. The two
// character sequences \r and \n stand in for them.
static std::string unescape(const std::string &value)
{
	std::string out;

	for (std::size_t i = 0; i < value.size(); ++i)
	{
		if (value[i] == '\\' && i + 1 < value.size())
		{
			if (value[i + 1] == 'r')
			{
				out += '\r';
				++i;
				continue;
			}
			if (value[i + 1] == 'n')
			{
				out += '\n';
				++i;
				continue;
			}
		}
		out += value[i];
	}
	return out;
}

static void applyHeaderDirective(HttpResponse &response,
                                 const std::string &value)
{
	std::size_t separator = value.find(':');

	if (separator == std::string::npos)
	{
		throw std::runtime_error("HEADER needs \"name: value\", got \"" +
		                         value + "\"");
	}
	std::string name = value.substr(0, separator);
	std::string headerValue = value.substr(separator + 1);
	if (!headerValue.empty() && headerValue[0] == ' ')
	{
		headerValue.erase(0, 1);
	}
	response.headers[name] = unescape(headerValue);
}

static void applyStatusDirective(HttpResponse &response,
                                 const std::string &value)
{
	std::istringstream valueStream(value);
	int statusCode;

	if (!(valueStream >> statusCode) || !valueStream.eof())
	{
		throw std::runtime_error("Invalid status code \"" + value + "\"");
	}
	response.statusCode = static_cast<HttpStatusCode>(statusCode);
}

static HttpResponse readResponseSpecification(const std::string &text)
{
	HttpResponse response;
	std::size_t offset = 0;

	while (offset < text.size())
	{
		std::string line = takeLine(text, offset);
		if (line.empty())
		{
			continue;
		}
		std::size_t space = line.find(' ');
		std::string directive = line.substr(0, space);
		std::string value =
			space == std::string::npos ? "" : line.substr(space + 1);

		if (directive == "BODY")
		{
			// Verbatim to the end, less one trailing newline if there is one.
			std::string body = text.substr(offset);
			if (!body.empty() && body[body.size() - 1] == '\n')
			{
				body.erase(body.size() - 1);
			}
			response.body = body;
			return response;
		}
		if (directive == "STATUS")
		{
			applyStatusDirective(response, value);
		}
		else if (directive == "VERSION")
		{
			response.version =
				HttpVersionHelpers::getHttpVersionForString(value);
		}
		else if (directive == "HEADER")
		{
			applyHeaderDirective(response, value);
		}
		else
		{
			throw std::runtime_error("Unknown directive \"" + directive + "\"");
		}
	}
	return response;
}

// Renders the exact bytes with each CR shown, so that a test can assert on
// framing rather than only on content.
static void printRaw(const std::string &built)
{
	for (std::size_t i = 0; i < built.size(); ++i)
	{
		if (built[i] == '\r')
		{
			std::cout << "\\r";
		}
		else if (built[i] == '\n')
		{
			std::cout << "\\n" << std::endl;
		}
		else
		{
			std::cout << built[i];
		}
	}
	std::cout << std::endl;
}

// The head must be CRLF-terminated throughout and separated from the body by
// one empty line. Reported as a single line so that every test checks framing
// without having to spell the bytes out.
static std::string describeFraming(const std::string &built)
{
	std::size_t headEnd = built.find("\r\n\r\n");

	if (headEnd == std::string::npos)
	{
		return "no CRLF CRLF separating head from body";
	}
	std::string head = built.substr(0, headEnd + 2);
	for (std::size_t i = 0; i < head.size(); ++i)
	{
		if (head[i] == '\n' && (i == 0 || head[i - 1] != '\r'))
		{
			return "bare LF in head";
		}
		if (head[i] == '\r' && (i + 1 >= head.size() || head[i + 1] != '\n'))
		{
			return "bare CR in head";
		}
	}
	return "OK";
}

// Date carries the current instant, so a test cannot pin its value. Checking
// the shape and printing a placeholder keeps the format under test without
// making the expected output change every second.
static bool isImfFixdate(const std::string &value)
{
	static const std::string DAY_NAMES = "Sun Mon Tue Wed Thu Fri Sat";
	static const std::string MONTH_NAMES =
		"Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec";
	static const std::size_t IMF_FIXDATE_LENGTH = 29;
	static const char *const DIGIT_POSITIONS = "     dd  "
											   "   dddd ddxddxdd    ";

	if (value.size() != IMF_FIXDATE_LENGTH)
	{
		return false;
	}
	if (DAY_NAMES.find(value.substr(0, 3)) == std::string::npos ||
	    MONTH_NAMES.find(value.substr(8, 3)) == std::string::npos)
	{
		return false;
	}
	if (value[3] != ',' || value[4] != ' ' || value[7] != ' ' ||
	    value[11] != ' ' || value[16] != ' ' || value[25] != ' ' ||
	    value.substr(26) != "GMT")
	{
		return false;
	}
	for (std::size_t i = 0; i < IMF_FIXDATE_LENGTH; ++i)
	{
		if (DIGIT_POSITIONS[i] == 'd' &&
		    !isdigit(static_cast<unsigned char>(value[i])))
		{
			return false;
		}
		if (DIGIT_POSITIONS[i] == 'x' && value[i] != ':')
		{
			return false;
		}
	}
	return true;
}

// Replaces a valid Date value with a fixed placeholder, and leaves anything
// else visible so that a malformed one fails its test rather than hiding.
static std::string normaliseHeaderLine(const std::string &line)
{
	static const std::string DATE_PREFIX = "Date: ";

	if (line.compare(0, DATE_PREFIX.size(), DATE_PREFIX) != 0)
	{
		return line;
	}
	std::string value = line.substr(DATE_PREFIX.size());
	return DATE_PREFIX + (isImfFixdate(value) ? "<IMF-fixdate>" : value);
}

static void printBuilt(const std::string &built)
{
	std::size_t headEnd = built.find("\r\n\r\n");
	std::string head =
		headEnd == std::string::npos ? built : built.substr(0, headEnd);
	std::string body =
		headEnd == std::string::npos ? "" : built.substr(headEnd + 4);
	std::size_t offset = 0;
	bool isStatusLine = true;

	while (offset < head.size())
	{
		std::string line = takeLine(head, offset);
		if (!line.empty() && line[line.size() - 1] == '\r')
		{
			line.erase(line.size() - 1);
		}
		if (isStatusLine)
		{
			std::cout << "STATUS-LINE: " << line << std::endl;
			std::cout << "HEADERS:" << std::endl;
			isStatusLine = false;
		}
		else
		{
			std::cout << "  " << normaliseHeaderLine(line) << std::endl;
		}
	}
	std::cout << "BODY: " << body.size() << " bytes" << std::endl;
	if (!body.empty())
	{
		std::cout << body << std::endl;
	}
	std::cout << "FRAMING: " << describeFraming(built) << std::endl;
}

int main(int argc, char **argv)
{
	std::string filePath;
	bool raw = false;

	try
	{
		for (int argumentIndex = 1; argumentIndex < argc; ++argumentIndex)
		{
			std::string argument = argv[argumentIndex];

			if (argument == "--raw")
			{
				raw = true;
			}
			else if (!argument.empty() && argument[0] != '-' &&
			         filePath.empty())
			{
				filePath = argument;
			}
			else
			{
				std::cerr << "Usage: " << argv[0] << USAGE_SUFFIX << std::endl;
				return EXIT_FAILURE;
			}
		}
		if (filePath.empty())
		{
			std::cerr << "Usage: " << argv[0] << USAGE_SUFFIX << std::endl;
			return EXIT_FAILURE;
		}

		HttpResponse response =
			readResponseSpecification(readWholeFile(filePath));
		std::string built = HttpResponseBuilder::build(response);

		if (raw)
		{
			printRaw(built);
		}
		else
		{
			printBuilt(built);
		}
	}
	catch (const std::exception &exception)
	{
		std::cerr << "An error occurred during execution:" << std::endl;
		std::cerr << exception.what() << "." << std::endl;
		std::cerr << "Unable to continue." << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
