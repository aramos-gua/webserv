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
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include "HttpResponseBuilder.hpp"
#include "HttpVersionHelpers.hpp"
#include "ResponseBodyReader.hpp"

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
static const char *const USAGE_SUFFIX =
	" [--raw] [--read-size <size>] <path to response file>";

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

// A body file is named relative to the response file that mentions it, so a
// fixture works wherever the suite is run from. Same rule the config parser
// uses for include.
static std::string resolveRelativeTo(const std::string &referencePath,
                                     const std::string &path)
{
	std::size_t lastSlash = referencePath.rfind('/');

	if ((!path.empty() && path[0] == '/') || lastSlash == std::string::npos)
	{
		return path;
	}
	return referencePath.substr(0, lastSlash + 1) + path;
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
	if (!response.setHeader(name, unescape(headerValue)))
	{
		throw std::runtime_error("Invalid header field name \"" + name + "\"");
	}
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
	response.setStatusCode(static_cast<HttpStatusCode>(statusCode));
}

static HttpResponse readResponseSpecification(const std::string &text,
                                              const std::string &specFilePath,
                                              std::string &bodyFilePath)
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

		if (directive == "BODYFILE")
		{
			// The response is told the length and nothing else. The descriptor
			// stays with this function's caller, which is exactly how Client
			// holds it: borrowed for the length of the send, owned elsewhere.
			std::string resolved = resolveRelativeTo(specFilePath, value);
			struct stat fileStatus;
			if (stat(resolved.c_str(), &fileStatus) != 0)
			{
				throw std::runtime_error("Couldn't stat body file \"" + value +
				                         "\"");
			}
			response.setExternalBody(
				static_cast<std::size_t>(fileStatus.st_size));
			bodyFilePath = resolved;
			continue;
		}
		if (directive == "BODY")
		{
			// Verbatim to the end, less one trailing newline if there is one.
			std::string body = text.substr(offset);
			if (!body.empty() && body[body.size() - 1] == '\n')
			{
				body.erase(body.size() - 1);
			}
			response.setBody(body);
			return response;
		}
		if (directive == "STATUS")
		{
			applyStatusDirective(response, value);
		}
		else if (directive == "VERSION")
		{
			response.setVersion(
				HttpVersionHelpers::getHttpVersionForString(value));
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
	std::size_t readSize = ResponseBodyReader::DEFAULT_READ_SIZE;

	try
	{
		for (int argumentIndex = 1; argumentIndex < argc; ++argumentIndex)
		{
			std::string argument = argv[argumentIndex];

			if (argument == "--raw")
			{
				raw = true;
			}
			else if (argument == "--read-size" && argumentIndex + 1 < argc)
			{
				std::istringstream valueStream(argv[++argumentIndex]);
				if (!(valueStream >> readSize) || !valueStream.eof() ||
				    readSize == 0)
				{
					throw std::runtime_error("Invalid value for --read-size");
				}
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

		std::string bodyFilePath;
		HttpResponse response = readResponseSpecification(
			readWholeFile(filePath), filePath, bodyFilePath);
		std::string built = HttpResponseBuilder::build(response);

		// Asks the response whether the body is external rather than whether a
		// file was named, so that a later BODY superseding a BODYFILE is
		// honoured here the same way the builder honours it.
		if (response.getWhetherBodyIsExternal() && !bodyFilePath.empty())
		{
			int bodyFileDescriptor = open(bodyFilePath.c_str(), O_RDONLY);
			if (bodyFileDescriptor < 0)
			{
				throw std::runtime_error("Couldn't open body file \"" +
				                         bodyFilePath + "\" for reading");
			}
			// Drained the way Client drains it: repeatedly, a bounded piece at
			// a time, until the promised length has been met. --read-size 1
			// forces the most fragmented case, as --chunk 1 does for requests.
			std::size_t remaining = response.getBodyLength();
			while (remaining > 0)
			{
				ssize_t appended = ResponseBodyReader::appendUpTo(
					bodyFileDescriptor, remaining, readSize, built);
				if (appended <= 0)
				{
					break;
				}
				remaining -= static_cast<std::size_t>(appended);
			}
			close(bodyFileDescriptor);
		}

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
