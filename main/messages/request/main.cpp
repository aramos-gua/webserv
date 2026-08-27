/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emflynn <emflynn@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:58:38 by aramos            #+#    #+#             */
/*   Updated: 2026/08/27 15:42:23 by emflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "HttpMethodHelpers.hpp"
#include "HttpRequestParser.hpp"
#include "HttpStatusCode.hpp"
#include "HttpStatusCodeHelpers.hpp"
#include "HttpVersionHelpers.hpp"

// Feeding the whole request in one go and feeding it a byte at a time have to
// produce identical results, so the chunk size is a command line option and the
// test suite runs every request file both ways.
static const char *const USAGE_SUFFIX =
	" [--chunk <size>] [--max-body-size <size>] <path to request file>";

static std::string readWholeFile(const std::string &filePath)
{
	std::ifstream fileStream(filePath.c_str(), std::ios::binary);

	if (!fileStream.is_open())
	{
		throw std::runtime_error("Couldn't open request file \"" + filePath +
		                         "\" for reading");
	}
	std::ostringstream contents;
	contents << fileStream.rdbuf();
	return contents.str();
}

static std::size_t parseSize(const std::string &option,
                             const std::string &value)
{
	std::istringstream valueStream(value);
	std::size_t size;

	if (!(valueStream >> size) || !valueStream.eof())
	{
		throw std::runtime_error("Invalid value \"" + value + "\" for " +
		                         option);
	}
	return size;
}

static void printRequest(const HttpRequest &request)
{
	std::cout << "METHOD: "
			  << HttpMethodHelpers::getStringForHttpMethod(request.method)
			  << std::endl;
	std::cout << "PATH: " << request.path << std::endl;
	std::cout << "VERSION: "
			  << HttpVersionHelpers::getStringForHttpVersion(request.version)
			  << std::endl;
	std::cout << "HEADERS:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator headerIterator =
	         request.headers.begin();
	     headerIterator != request.headers.end(); ++headerIterator)
	{
		// Values are quoted so that empty ones, and any leading or trailing
		// whitespace the parser was meant to strip, are visible in the dump.
		std::cout << "  " << headerIterator->first << ": \""
				  << headerIterator->second << "\"" << std::endl;
	}
	std::cout << "BODY: " << request.body.size() << " bytes" << std::endl;
	if (!request.body.empty())
	{
		std::cout << request.body << std::endl;
	}
}

static void printUnparsedByteCount(const HttpRequestParser &parser)
{
	if (parser.getUnparsedByteCount() > 0)
	{
		std::cout << "LEFTOVER: " << parser.getUnparsedByteCount() << " bytes"
				  << std::endl;
	}
}

int main(int argc, char **argv)
{
	std::string filePath;
	std::size_t chunkSize = 0;
	std::size_t maxBodySize = 0;
	bool maxBodySizeGiven = false;

	try
	{
		for (int argumentIndex = 1; argumentIndex < argc; ++argumentIndex)
		{
			std::string argument = argv[argumentIndex];
			bool isLastArgument = argumentIndex + 1 == argc;

			if (argument == "--chunk" && !isLastArgument)
			{
				chunkSize = parseSize(argument, argv[++argumentIndex]);
			}
			else if (argument == "--max-body-size" && !isLastArgument)
			{
				maxBodySize = parseSize(argument, argv[++argumentIndex]);
				maxBodySizeGiven = true;
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

		std::string rawRequest = readWholeFile(filePath);
		HttpRequestParser parser = maxBodySizeGiven
		                               ? HttpRequestParser(maxBodySize)
		                               : HttpRequestParser();
		HttpRequestParser::Result result = HttpRequestParser::INCOMPLETE;

		// A chunk size of zero means feeding the whole request at once.
		std::size_t step = chunkSize == 0 ? rawRequest.size() : chunkSize;
		if (step == 0)
		{
			result = parser.feed("", 0);
		}
		for (std::size_t offset = 0; offset < rawRequest.size(); offset += step)
		{
			std::size_t remaining = rawRequest.size() - offset;
			result = parser.feed(rawRequest.data() + offset,
			                     remaining < step ? remaining : step);
		}

		if (result == HttpRequestParser::COMPLETE)
		{
			std::cout << "RESULT: COMPLETE" << std::endl;
			printRequest(parser.getRequest());
			printUnparsedByteCount(parser);
		}
		else if (result == HttpRequestParser::INVALID)
		{
			HttpStatusCode statusCode = parser.getErrorStatusCode();
			std::cout << "RESULT: INVALID" << std::endl;
			std::cout << "STATUS: " << static_cast<int>(statusCode) << " "
					  << HttpStatusCodeHelpers::getReasonPhraseForStatusCode(
							 statusCode)
					  << std::endl;
		}
		else
		{
			std::cout << "RESULT: INCOMPLETE" << std::endl;
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
