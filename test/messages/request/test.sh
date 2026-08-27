#!/usr/bin/env bash

export GREEN="\033[32m"
export RED="\033[31m"
export DEFAULT="\033[0m"

export REQUESTS_DIR="../../../test/messages/request/request-files"
export PARSER="../../../bin/messages-request"

# Check that feeding a request file to the parser produces the expected dump,
# both when the whole request arrives in one read and when it arrives a byte at
# a time. How a request is split across reads must never change the outcome.
run_request_test() {
	REQUEST_FILE="$REQUESTS_DIR/$REQUEST_FILE"
	export WHOLE="$($PARSER $OPTIONS $REQUEST_FILE 2>&1)"
	export CHUNKED="$($PARSER --chunk 1 $OPTIONS $REQUEST_FILE 2>&1)"
	if ! cmp -s <(echo "$EXPECTED") <(echo "$WHOLE"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& diff <(echo "$EXPECTED") <(echo "$WHOLE")
	elif ! cmp -s <(echo "$WHOLE") <(echo "$CHUNKED"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "Byte-at-a-time delivery gave a different result:" \
			&& diff <(echo "$WHOLE") <(echo "$CHUNKED")
	else
		echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME"
	fi
	export OPTIONS=""
}

# As above, but comparing only the result and status lines, for requests whose
# full dump would be impractically large to write out
run_result_test() {
	REQUEST_FILE="$REQUESTS_DIR/$REQUEST_FILE"
	export WHOLE="$($PARSER $OPTIONS $REQUEST_FILE 2>&1 \
		| grep -E '^(RESULT|STATUS):')"
	export CHUNKED="$($PARSER --chunk 1 $OPTIONS $REQUEST_FILE 2>&1 \
		| grep -E '^(RESULT|STATUS):')"
	if ! cmp -s <(echo "$EXPECTED") <(echo "$WHOLE"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& diff <(echo "$EXPECTED") <(echo "$WHOLE")
	elif ! cmp -s <(echo "$WHOLE") <(echo "$CHUNKED"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "Byte-at-a-time delivery gave a different result:" \
			&& diff <(echo "$WHOLE") <(echo "$CHUNKED")
	else
		echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME"
	fi
	export OPTIONS=""
}

# Check that a bad invocation produces the expected usage or error message
run_usage_test() {
	export ACTUAL="$($PARSER $OPTIONS 2>&1)"
	cmp -s <(echo "$EXPECTED") <(echo "$ACTUAL") \
		&& (echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME") \
		|| (echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "$ACTUAL")
	export OPTIONS=""
}

echo "*** USAGE TESTS ***"
echo -n "Run test suite? [Y/n] "
read -n 1 ANSWER
echo

if [ "$ANSWER" != "n" ]; then
	export TEST_NAME="Missing request file"
	export OPTIONS=""
	export EXPECTED="\
Usage: $PARSER [--chunk <size>] [--max-body-size <size>] <path to request file>"
	run_usage_test

	export TEST_NAME="Unknown option"
	export OPTIONS="--nonsense $REQUESTS_DIR/simple-get.http"
	export EXPECTED="\
Usage: $PARSER [--chunk <size>] [--max-body-size <size>] <path to request file>"
	run_usage_test

	export TEST_NAME="Non-numeric chunk size"
	export OPTIONS="--chunk abc $REQUESTS_DIR/simple-get.http"
	export EXPECTED="\
An error occurred during execution:
Invalid value \"abc\" for --chunk.
Unable to continue."
	run_usage_test

	export TEST_NAME="Nonexistent request file"
	export OPTIONS="$REQUESTS_DIR/nonexistent.http"
	export EXPECTED="\
An error occurred during execution:
Couldn't open request file \"$REQUESTS_DIR/nonexistent.http\" for reading.
Unable to continue."
	run_usage_test
fi

echo
echo "*** SUCCESS TESTS ***"
echo -n "Run test suite? [Y/n] "
read -n 1 ANSWER
echo

if [ "$ANSWER" != "n" ]; then
	export TEST_NAME="Simple GET request"
	export REQUEST_FILE="simple-get.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
BODY: 0 bytes"
	run_request_test

	export TEST_NAME="Request with no headers at all"
	export REQUEST_FILE="no-headers.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /
VERSION: HTTP/1.0
HEADERS:
BODY: 0 bytes"
	run_request_test

	export TEST_NAME="POST request with a body"
	export REQUEST_FILE="post-with-body.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /submit
VERSION: HTTP/1.1
HEADERS:
  content-length: \"11\"
  content-type: \"text/plain\"
  host: \"example.com\"
BODY: 11 bytes
hello world"
	run_request_test

	export TEST_NAME="Header names lowercased, values trimmed, empty value kept"
	export REQUEST_FILE="header-case-and-whitespace.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /mixed
VERSION: HTTP/1.1
HEADERS:
  host: \"Example.COM\"
  user-agent: \"spaced-out\"
  x-empty: \"\"
BODY: 0 bytes"
	run_request_test

	export TEST_NAME="Content-Length of zero"
	export REQUEST_FILE="zero-content-length.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /empty
VERSION: HTTP/1.1
HEADERS:
  content-length: \"0\"
  host: \"example.com\"
BODY: 0 bytes"
	run_request_test

	# Repeating a list-valued field means the same as one field line with the
	# values joined by commas, in the order they arrived.
	export TEST_NAME="Repeated list field is joined in order"
	export REQUEST_FILE="duplicate-list-field.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /
VERSION: HTTP/1.1
HEADERS:
  accept: \"text/html, application/json, text/plain\"
  host: \"example.com\"
BODY: 0 bytes"
	run_request_test

	export TEST_NAME="Repeated list field whose values already hold commas"
	export REQUEST_FILE="duplicate-list-field-with-commas.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /
VERSION: HTTP/1.1
HEADERS:
  accept-encoding: \"gzip, deflate, br\"
  host: \"example.com\"
BODY: 0 bytes"
	run_request_test

	# Repeating a list field keeps both values rather than deduplicating them:
	# for X-Forwarded-For the same address twice is real information.
	export TEST_NAME="Repeated list field with identical values is kept twice"
	export REQUEST_FILE="duplicate-list-field-identical.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /
VERSION: HTTP/1.1
HEADERS:
  host: \"abc.com\"
  x-forwarded-for: \"10.0.0.1, 10.0.0.1\"
BODY: 0 bytes"
	run_request_test

	# The same applies to an empty Host: HTTP/1.0 imposes no requirement on it
	# either way, so the field is kept as it arrived.
	export TEST_NAME="Empty Host on HTTP/1.0 is accepted"
	export REQUEST_FILE="empty-host-http-1-0.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /
VERSION: HTTP/1.0
HEADERS:
  accept: \"*/*\"
  host: \"\"
BODY: 0 bytes"
	run_request_test

	# HTTP/1.0 predates virtual hosting, so a missing Host is fine there. This
	# is the negative control for the HTTP/1.1 requirement below.
	export TEST_NAME="No Host on HTTP/1.0 is accepted"
	export REQUEST_FILE="missing-host-http-1-0.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /
VERSION: HTTP/1.0
HEADERS:
  accept: \"*/*\"
  user-agent: \"curl\"
BODY: 0 bytes"
	run_request_test

	# Every unusual character that RFC 9110 does allow in a field name, to
	# prove the token check has not over-rejected. The backtick and dollar are
	# escaped for the shell, not because the parser cares.
	export TEST_NAME="Field name using every permitted token symbol"
	export REQUEST_FILE="header-name-all-token-symbols.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
  x-odd!#\$%&'*+-.^_\`|~9: \"fine\"
BODY: 0 bytes"
	run_request_test

	# A method other than GET, to show the enum round-trips rather than the
	# dump simply echoing back whatever string arrived.
	export TEST_NAME="DELETE request"
	export REQUEST_FILE="delete-request.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: DELETE
PATH: /resource
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
BODY: 0 bytes"
	run_request_test

	export TEST_NAME="Chunked body reassembled from several chunks"
	export REQUEST_FILE="chunked-body.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /upload
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
  transfer-encoding: \"chunked\"
BODY: 11 bytes
hello world"
	run_request_test

	export TEST_NAME="Chunked body with no chunks at all"
	export REQUEST_FILE="chunked-body-empty.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /upload
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
  transfer-encoding: \"chunked\"
BODY: 0 bytes"
	run_request_test

	export TEST_NAME="Chunk size given in uppercase hexadecimal"
	export REQUEST_FILE="chunked-body-uppercase-hex.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /upload
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
  transfer-encoding: \"chunked\"
BODY: 10 bytes
0123456789"
	run_request_test


	export TEST_NAME="Trailer fields are discarded, not merged into headers"
	export REQUEST_FILE="chunked-body-with-trailers.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /upload
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
  transfer-encoding: \"chunked\"
BODY: 5 bytes
hello"
	run_request_test

	export TEST_NAME="Transfer encoding named in uppercase"
	export REQUEST_FILE="chunked-transfer-encoding-uppercase.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /upload
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
  transfer-encoding: \"CHUNKED\"
BODY: 5 bytes
hello"
	run_request_test

	export TEST_NAME="Request pipelined behind a chunked body"
	export REQUEST_FILE="chunked-body-then-pipelined.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /upload
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
  transfer-encoding: \"chunked\"
BODY: 5 bytes
hello
LEFTOVER: 43 bytes"
	run_request_test

	# Bytes arriving after a complete request belong to the next request on the
	# connection and must survive both feed() and reset(). The count is exactly
	# the length of the second request below.
	export TEST_NAME="Pipelined request is kept as leftover"
	export REQUEST_FILE="pipelined-requests.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: GET
PATH: /first
VERSION: HTTP/1.1
HEADERS:
  host: \"example.com\"
BODY: 0 bytes
LEFTOVER: 43 bytes"
	run_request_test

	export TEST_NAME="Pipelined request following a body is kept as leftover"
	export REQUEST_FILE="pipelined-after-body.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /upload
VERSION: HTTP/1.1
HEADERS:
  content-length: \"11\"
  host: \"example.com\"
BODY: 11 bytes
hello world
LEFTOVER: 43 bytes"
	run_request_test

	# A CRLF inside the body must be taken as body content, not as the end of
	# anything, so the expected body is spelled out with a real carriage return.
	export TEST_NAME="Body containing a CRLF is not truncated"
	export REQUEST_FILE="body-with-crlf.http"
	export EXPECTED="\
RESULT: COMPLETE
METHOD: POST
PATH: /binary
VERSION: HTTP/1.1
HEADERS:
  content-length: \"12\"
  host: \"example.com\"
BODY: 12 bytes
line1"$'\r'"
line2"
	run_request_test
fi

echo
echo "*** INCOMPLETE REQUEST TESTS ***"
echo -n "Run test suite? [Y/n] "
read -n 1 ANSWER
echo

if [ "$ANSWER" != "n" ]; then
	export TEST_NAME="Empty input"
	export REQUEST_FILE="empty.http"
	export EXPECTED="RESULT: INCOMPLETE"
	run_request_test

	export TEST_NAME="Headers never terminated"
	export REQUEST_FILE="truncated-headers.http"
	export EXPECTED="RESULT: INCOMPLETE"
	run_request_test

	export TEST_NAME="Chunk data shorter than its declared size"
	export REQUEST_FILE="chunked-truncated-data.http"
	export EXPECTED="RESULT: INCOMPLETE"
	run_request_test

	export TEST_NAME="Chunked body with no terminating zero chunk"
	export REQUEST_FILE="chunked-no-last-chunk.http"
	export EXPECTED="RESULT: INCOMPLETE"
	run_request_test

	export TEST_NAME="Body shorter than Content-Length"
	export REQUEST_FILE="truncated-body.http"
	export EXPECTED="RESULT: INCOMPLETE"
	run_request_test
fi

echo
echo "*** ERROR TESTS ***"
echo -n "Run test suite? [Y/n] "
read -n 1 ANSWER
echo

if [ "$ANSWER" != "n" ]; then
	export TEST_NAME="Request line with too few fields"
	export REQUEST_FILE="invalid-request-line.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Empty request line"
	export REQUEST_FILE="empty-request-line.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	# The request line is exactly "method SP target SP version": no fourth
	# field, no repeated or stray spaces, no missing field.
	export TEST_NAME="Request line with trailing garbage"
	export REQUEST_FILE="request-line-trailing-garbage.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Request line with a doubled space"
	export REQUEST_FILE="request-line-double-space.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Request line with a leading space"
	export REQUEST_FILE="request-line-leading-space.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Request line with no version"
	export REQUEST_FILE="request-line-missing-version.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	# A token HTTP does not define at all: the request line is bad, so 400.
	export TEST_NAME="Unrecognised method"
	export REQUEST_FILE="unknown-method.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	# Methods are case-sensitive, so "get" is not GET and is not recognised.
	export TEST_NAME="Lowercase method"
	export REQUEST_FILE="lowercase-method.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	# Methods HTTP does define but this server does not act on: 501, which is
	# what separates "we have not built this" from "that is not a method".
	export TEST_NAME="Recognised but unimplemented method: HEAD"
	export REQUEST_FILE="unimplemented-method-head.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 501 Not Implemented"
	run_request_test

	export TEST_NAME="Recognised but unimplemented method: PUT"
	export REQUEST_FILE="unimplemented-method-put.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 501 Not Implemented"
	run_request_test

	export TEST_NAME="Recognised but unimplemented method: PATCH"
	export REQUEST_FILE="unimplemented-method-patch.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 501 Not Implemented"
	run_request_test

	export TEST_NAME="Recognised but unimplemented method: CONNECT"
	export REQUEST_FILE="unimplemented-method-connect.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 501 Not Implemented"
	run_request_test

	export TEST_NAME="Recognised but unimplemented method: OPTIONS"
	export REQUEST_FILE="unimplemented-method-options.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 501 Not Implemented"
	run_request_test

	export TEST_NAME="Recognised but unimplemented method: TRACE"
	export REQUEST_FILE="unimplemented-method-trace.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 501 Not Implemented"
	run_request_test

	export TEST_NAME="Unsupported HTTP version"
	export REQUEST_FILE="unsupported-version.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 505 HTTP Version Not Supported"
	run_request_test

	export TEST_NAME="HTTP version that only looks like a supported one"
	export REQUEST_FILE="version-near-miss.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	# The version token is case-sensitive too, so "http/1.1" is not a version
	# HTTP defines at all.
	export TEST_NAME="Lowercase HTTP version"
	export REQUEST_FILE="lowercase-version.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Recognised but unsupported version: HTTP/2"
	export REQUEST_FILE="recognised-unsupported-version-http-2.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 505 HTTP Version Not Supported"
	run_request_test

	export TEST_NAME="Recognised but unsupported version: HTTP/3"
	export REQUEST_FILE="recognised-unsupported-version-http-3.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 505 HTTP Version Not Supported"
	run_request_test

	export TEST_NAME="Header line with no colon"
	export REQUEST_FILE="malformed-header-line.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Whitespace between field name and colon"
	export REQUEST_FILE="header-name-space-before-colon.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Space inside a field name"
	export REQUEST_FILE="header-name-internal-space.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Empty field name"
	export REQUEST_FILE="header-name-empty.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Field name containing a non-token character"
	export REQUEST_FILE="header-name-non-token-character.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Obsolete line folding is rejected, not tidied up"
	export REQUEST_FILE="header-obsolete-line-folding.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Non-numeric Content-Length"
	export REQUEST_FILE="non-numeric-content-length.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Negative Content-Length"
	export REQUEST_FILE="negative-content-length.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Content-Length"
	export REQUEST_FILE="duplicate-content-length.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Host"
	export REQUEST_FILE="duplicate-host.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Missing Host on HTTP/1.1"
	export REQUEST_FILE="missing-host.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	# A Host that is present but empty names no authority, which RFC 9112
	# counts as an invalid field value rather than a present one.
	export TEST_NAME="Empty Host on HTTP/1.1"
	export REQUEST_FILE="empty-host.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	# It is the count of Host lines that is illegal, not their disagreeing.
	export TEST_NAME="Duplicate Host carrying identical values"
	export REQUEST_FILE="duplicate-host-identical.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate User-Agent, whose tokens commas would corrupt"
	export REQUEST_FILE="duplicate-user-agent.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Cookie, whose values are semicolon-separated"
	export REQUEST_FILE="duplicate-cookie.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Authorization"
	export REQUEST_FILE="duplicate-authorization.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Date"
	export REQUEST_FILE="duplicate-date.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate From"
	export REQUEST_FILE="duplicate-from.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Origin"
	export REQUEST_FILE="duplicate-origin.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Referer"
	export REQUEST_FILE="duplicate-referer.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Content-Type"
	export REQUEST_FILE="duplicate-content-type.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Transfer-Encoding together with Content-Length"
	export REQUEST_FILE="chunked-with-content-length.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Transfer encoding other than chunked"
	export REQUEST_FILE="unsupported-transfer-encoding.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 501 Not Implemented"
	run_request_test

	export TEST_NAME="Unrecognised transfer encoding"
	export REQUEST_FILE="unrecognised-transfer-encoding.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Transfer encoding list"
	export REQUEST_FILE="transfer-encoding-list.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 501 Not Implemented"
	run_request_test

	export TEST_NAME="Empty Transfer-Encoding value"
	export REQUEST_FILE="transfer-encoding-empty.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Chunk extensions are rejected"
	export REQUEST_FILE="chunked-body-with-extensions.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Chunk size that is not hexadecimal"
	export REQUEST_FILE="chunked-bad-size.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Chunk not terminated by CRLF"
	export REQUEST_FILE="chunked-missing-terminator.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Chunked body on an HTTP/1.0 request"
	export REQUEST_FILE="chunked-on-http-1-0.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Malformed trailer field"
	export REQUEST_FILE="chunked-malformed-trailer.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Chunked body larger than the configured limit"
	export REQUEST_FILE="chunked-body.http"
	export OPTIONS="--max-body-size 8"
	export EXPECTED="\
RESULT: INVALID
STATUS: 413 Content Too Large"
	run_request_test

	export TEST_NAME="Body larger than the configured limit"
	export REQUEST_FILE="body-too-large.http"
	export OPTIONS="--max-body-size 10"
	export EXPECTED="\
RESULT: INVALID
STATUS: 413 Content Too Large"
	run_request_test

	export TEST_NAME="More headers than the parser accepts"
	export REQUEST_FILE="too-many-headers.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 431 Request Header Fields Too Large"
	run_result_test

	export TEST_NAME="Oversized request line"
	export REQUEST_FILE="request-line-too-large.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 414 URI Too Long"
	run_result_test

	export TEST_NAME="Oversized header line"
	export REQUEST_FILE="headers-too-large.http"
	export EXPECTED="\
RESULT: INVALID
STATUS: 431 Request Header Fields Too Large"
	run_result_test

	# The limit is inclusive, and a line of exactly the maximum length must be
	# accepted however its CR and LF happen to fall across reads.
	export TEST_NAME="Request line of exactly the maximum length"
	export REQUEST_FILE="request-line-at-limit.http"
	export EXPECTED="RESULT: COMPLETE"
	run_result_test

	export TEST_NAME="Header line of exactly the maximum length"
	export REQUEST_FILE="header-line-at-limit.http"
	export EXPECTED="RESULT: COMPLETE"
	run_result_test
fi
