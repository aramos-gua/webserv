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
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Empty request line"
	export REQUEST_FILE="empty-request-line.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Header line with no colon"
	export REQUEST_FILE="malformed-header-line.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Whitespace between field name and colon"
	export REQUEST_FILE="header-name-space-before-colon.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Space inside a field name"
	export REQUEST_FILE="header-name-internal-space.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Empty field name"
	export REQUEST_FILE="header-name-empty.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Field name containing a non-token character"
	export REQUEST_FILE="header-name-non-token-character.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Obsolete line folding is rejected, not tidied up"
	export REQUEST_FILE="header-obsolete-line-folding.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Non-numeric Content-Length"
	export REQUEST_FILE="non-numeric-content-length.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Negative Content-Length"
	export REQUEST_FILE="negative-content-length.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Duplicate Content-Length"
	export REQUEST_FILE="duplicate-content-length.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 400 Bad Request"
	run_request_test

	export TEST_NAME="Body larger than the configured limit"
	export REQUEST_FILE="body-too-large.http"
	export OPTIONS="--max-body-size 10"
	export EXPECTED="\
RESULT: ERROR
STATUS: 413 Content Too Large"
	run_request_test

	export TEST_NAME="More headers than the parser accepts"
	export REQUEST_FILE="too-many-headers.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 431 Request Header Fields Too Large"
	run_result_test

	export TEST_NAME="Oversized request line"
	export REQUEST_FILE="request-line-too-large.http"
	export EXPECTED="\
RESULT: ERROR
STATUS: 414 URI Too Long"
	run_result_test

	export TEST_NAME="Oversized header line"
	export REQUEST_FILE="headers-too-large.http"
	export EXPECTED="\
RESULT: ERROR
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
