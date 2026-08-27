#!/usr/bin/env bash

export GREEN="\033[32m"
export RED="\033[31m"
export DEFAULT="\033[0m"

export RESPONSES_DIR="../../../test/messages/response/response-files"
export BUILDER="../../../bin/messages-response"

# A fixture read with --raw must supply its own Date header. The generated one
# carries the current instant, so an expectation holding it goes stale a second
# later; the dumped form replaces it with a placeholder, but --raw is byte-exact
# on purpose.
#
# Check that building a described response produces the expected dump. Every
# dump ends in a FRAMING line, so each test also asserts that the head is
# CRLF-terminated throughout and separated from the body by one empty line.
run_response_test() {
	RESPONSE_FILE="$RESPONSES_DIR/$RESPONSE_FILE"
	export ACTUAL="$($BUILDER $OPTIONS $RESPONSE_FILE 2>&1)"
	if ! cmp -s <(echo "$EXPECTED") <(echo "$ACTUAL"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& diff <(echo "$EXPECTED") <(echo "$ACTUAL")
	else
		echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME"
	fi
	export OPTIONS=""
}

# As above, for a response whose body comes from a file. Also requires that
# reading that body one byte at a time gives exactly the same result: how the
# body is split across reads must never change the response, which is the same
# invariant --chunk 1 enforces on the request side.
run_body_test() {
	RESPONSE_FILE="$RESPONSES_DIR/$RESPONSE_FILE"
	export WHOLE="$($BUILDER $OPTIONS $RESPONSE_FILE 2>&1)"
	export FRAGMENTED="$($BUILDER --read-size 1 $OPTIONS $RESPONSE_FILE 2>&1)"
	if ! cmp -s <(echo "$EXPECTED") <(echo "$WHOLE"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& diff <(echo "$EXPECTED") <(echo "$WHOLE")
	elif ! cmp -s <(echo "$WHOLE") <(echo "$FRAGMENTED"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "Reading the body one byte at a time gave a different result:" \
			&& diff <(echo "$WHOLE") <(echo "$FRAGMENTED")
	else
		echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME"
	fi
	export OPTIONS=""
}

# As run_body_test, but comparing everything except the body content, for a
# body too large to write out in full
run_body_summary_test() {
	RESPONSE_FILE="$RESPONSES_DIR/$RESPONSE_FILE"
	export SUMMARISE='/^BODY: /{print; skip=1; next} /^FRAMING: /{skip=0} !skip'
	export WHOLE="$($BUILDER $OPTIONS $RESPONSE_FILE 2>&1 | awk "$SUMMARISE")"
	export FRAGMENTED="$($BUILDER --read-size 1 $OPTIONS $RESPONSE_FILE 2>&1 \
		| awk "$SUMMARISE")"
	if ! cmp -s <(echo "$EXPECTED") <(echo "$WHOLE"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& diff <(echo "$EXPECTED") <(echo "$WHOLE")
	elif ! cmp -s <(echo "$WHOLE") <(echo "$FRAGMENTED"); then
		echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "Reading the body one byte at a time gave a different result:" \
			&& diff <(echo "$WHOLE") <(echo "$FRAGMENTED")
	else
		echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME"
	fi
	export OPTIONS=""
}

# Check that a bad invocation produces the expected usage or error message
run_usage_test() {
	export ACTUAL="$($BUILDER $OPTIONS 2>&1)"
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
	export TEST_NAME="Missing response file"
	export OPTIONS=""
	export EXPECTED="\
Usage: $BUILDER [--raw] [--read-size <size>] <path to response file>"
	run_usage_test

	export TEST_NAME="Unknown option"
	export OPTIONS="--nonsense $RESPONSES_DIR/simple-ok.response"
	export EXPECTED="\
Usage: $BUILDER [--raw] [--read-size <size>] <path to response file>"
	run_usage_test

	export TEST_NAME="Nonexistent response file"
	export OPTIONS="$RESPONSES_DIR/does-not-exist.response"
	export EXPECTED="\
An error occurred during execution:
Couldn't open response file \"$RESPONSES_DIR/does-not-exist.response\" for reading.
Unable to continue."
	run_usage_test

	echo
	echo "*** BUILDING TESTS ***"
	export TEST_NAME="Simple 200 with a body"
	export RESPONSE_FILE="simple-ok.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Content-Type: text/plain
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 5
BODY: 5 bytes
hello
FRAMING: OK"
	run_response_test

	export TEST_NAME="404 with a body"
	export RESPONSE_FILE="not-found.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 404 Not Found
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 9
BODY: 9 bytes
not found
FRAMING: OK"
	run_response_test

	export TEST_NAME="200 with an empty body still carries Content-Length"
	export RESPONSE_FILE="empty-body.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 0
BODY: 0 bytes
FRAMING: OK"
	run_response_test

	export TEST_NAME="An explicit Connection header is left alone"
	export RESPONSE_FILE="explicit-connection.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: keep-alive
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 2
BODY: 2 bytes
hi
FRAMING: OK"
	run_response_test

	echo
	echo "*** CONTENT-LENGTH TESTS ***"
	# The builder computes Content-Length itself, so any the caller supplied has
	# to be dropped however it was spelled. Field names are case-insensitive, and
	# two Content-Length headers in one response is what lets a recipient and the
	# next hop disagree about where the body ends.
	export TEST_NAME="Caller's Content-Length is dropped: canonical spelling"
	export RESPONSE_FILE="content-length-canonical.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 5
BODY: 5 bytes
hello
FRAMING: OK"
	run_response_test

	export TEST_NAME="Caller's Content-Length is dropped: lowercase spelling"
	export RESPONSE_FILE="content-length-lowercase.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 5
BODY: 5 bytes
hello
FRAMING: OK"
	run_response_test

	export TEST_NAME="Caller's Content-Length is dropped: mixed-case spelling"
	export RESPONSE_FILE="content-length-mixed-case.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 5
BODY: 5 bytes
hello
FRAMING: OK"
	run_response_test

	export TEST_NAME="Caller's Content-Length is dropped: uppercase spelling"
	export RESPONSE_FILE="content-length-uppercase.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 5
BODY: 5 bytes
hello
FRAMING: OK"
	run_response_test

	export TEST_NAME="Connection is matched case-insensitively too"
	export RESPONSE_FILE="connection-uppercase.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  CONNECTION: keep-alive
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 2
BODY: 2 bytes
hi
FRAMING: OK"
	run_response_test

	echo
	echo "*** BODILESS STATUS CODE TESTS ***"
	# 204 and 304 cannot carry a body, so they carry no Content-Length either.
	export TEST_NAME="204 No Content has no Content-Length"
	export RESPONSE_FILE="no-content.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 204 No Content
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
BODY: 0 bytes
FRAMING: OK"
	run_response_test

	export TEST_NAME="304 Not Modified has no Content-Length"
	export RESPONSE_FILE="not-modified.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 304 Not Modified
HEADERS:
  ETag: \"abc\"
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
BODY: 0 bytes
FRAMING: OK"
	run_response_test

	echo
	echo "*** VERSION TESTS ***"
	export TEST_NAME="An explicit HTTP/1.0 is preserved"
	export RESPONSE_FILE="version-http-1-0.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.0 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 0
BODY: 0 bytes
FRAMING: OK"
	run_response_test

	export TEST_NAME="HTTP/2.0 is emitted in its canonical HTTP/2 spelling"
	export RESPONSE_FILE="version-http-2-with-minor.response"
	export EXPECTED="\
STATUS-LINE: HTTP/2 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 0
BODY: 0 bytes
FRAMING: OK"
	run_response_test

	# Rejected while reading the response file, before build() is reached.
	export TEST_NAME="An unrecognised version is rejected"
	export OPTIONS="$RESPONSES_DIR/version-unknown.response"
	export EXPECTED="\
An error occurred during execution:
Unknown HTTP version \"HTTP/9.9\".
Unable to continue."
	run_usage_test

	export TEST_NAME="Default version when none is given"
	export RESPONSE_FILE="default-version.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 0
BODY: 0 bytes
FRAMING: OK"
	run_response_test

	echo
	echo "*** DEFAULT HEADER TESTS ***"
	# Date and Server are supplied only when the caller has not spoken for
	# itself. The Date value is replaced with a placeholder by the dump binary
	# once it has been checked against the IMF-fixdate shape, since its real
	# value changes every second.
	# Read raw rather than dumped, so the caller's own value is visible: a
	# normalised Date would look identical whether it survived or was replaced.
	export TEST_NAME="A caller-supplied Date wins and is not duplicated"
	export RESPONSE_FILE="caller-date.response"
	export OPTIONS="--raw"
	export EXPECTED="\
HTTP/1.1 200 OK\\r\\n
Date: Sun, 06 Nov 1994 08:49:37 GMT\\r\\n
Connection: close\\r\\n
Server: penguinx\\r\\n
Content-Length: 0\\r\\n
\\r\\n"
	run_response_test

	export TEST_NAME="A caller-supplied Server wins and is not duplicated"
	export RESPONSE_FILE="caller-server.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Server: something-else
  Connection: close
  Date: <IMF-fixdate>
  Content-Length: 0
BODY: 0 bytes
FRAMING: OK"
	run_response_test

	echo
	echo "*** STATUS CODE TESTS ***"
	# The reason phrase lookup throws for a code it does not know, and build()
	# is on the path every response takes. An unknown code must degrade to an
	# empty phrase rather than unwind an exception out of the poll loop.
	export TEST_NAME="Unknown status code degrades to an empty reason phrase"
	export RESPONSE_FILE="unknown-status-code.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 299 
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 0
BODY: 0 bytes
FRAMING: OK"
	run_response_test

	echo
	echo "*** HEADER SANITISING TESTS ***"
	# Values are stripped of CR and LF; names are refused outright. A CRLF in a
	# name would append header lines of the setter's choosing, which matters
	# most once CGI scripts start supplying header names of their own.
	export TEST_NAME="A header name that is not a valid field name is refused"
	export OPTIONS="$RESPONSES_DIR/invalid-header-name.response"
	export EXPECTED="\
An error occurred during execution:
Invalid header field name \"Has Space\".
Unable to continue."
	run_usage_test

	export TEST_NAME="A header name carrying a CRLF is refused"
	export OPTIONS="$RESPONSES_DIR/header-name-with-crlf.response"
	export EXPECTED="\
An error occurred during execution:
Invalid header field name \"X-Ok\\r\\nInjected\".
Unable to continue."
	run_usage_test

	# A CR or LF in a header value would otherwise end the header line early and
	# let the value inject headers of its own, which is response splitting.
	export TEST_NAME="CR and LF are stripped from a header value"
	export RESPONSE_FILE="header-value-with-crlf.response"
	export OPTIONS="--raw"
	export EXPECTED="\
HTTP/1.1 200 OK\\r\\n
Date: Sun, 06 Nov 1994 08:49:37 GMT\\r\\n
X-Evil: aInjected: yes\\r\\n
Connection: close\\r\\n
Server: penguinx\\r\\n
Content-Length: 2\\r\\n
\\r\\n
hi"
	run_response_test

	export TEST_NAME="A body on a 204 is dropped, not left unframed"
	export RESPONSE_FILE="no-content-with-body.response"
	export OPTIONS="--raw"
	export EXPECTED="\
HTTP/1.1 204 No Content\\r\\n
Date: Sun, 06 Nov 1994 08:49:37 GMT\\r\\n
Connection: close\\r\\n
Server: penguinx\\r\\n
\\r\\n"
	run_response_test

	echo
	echo "*** EXTERNAL BODY TESTS ***"
	# The response records only the length of a body it does not hold, and the
	# caller streams the bytes from the descriptor it owns. Content-Length must
	# come from the file rather than from anything in the response.
	export TEST_NAME="Body streamed from a file"
	export RESPONSE_FILE="body-from-file.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Content-Type: text/plain
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 17
BODY: 17 bytes
hello from a file
FRAMING: OK"
	run_body_test

	export TEST_NAME="Empty external body"
	export RESPONSE_FILE="body-from-empty-file.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 0
BODY: 0 bytes
FRAMING: OK"
	run_body_test

	# An inline body set after an external one must clear it completely: the
	# response cannot both name a length it does not hold and hold bytes of
	# its own. large.txt is 20000 bytes, so a Content-Length of 5 is the
	# assertion that the external length really was discarded.
	export TEST_NAME="An inline body supersedes an external one"
	export RESPONSE_FILE="body-file-then-inline.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 5
BODY: 5 bytes
short
FRAMING: OK"
	run_body_test

	export TEST_NAME="External body larger than one read"
	export RESPONSE_FILE="body-from-large-file.response"
	export EXPECTED="\
STATUS-LINE: HTTP/1.1 200 OK
HEADERS:
  Connection: close
  Date: <IMF-fixdate>
  Server: penguinx
  Content-Length: 20000
BODY: 20000 bytes
FRAMING: OK"
	run_body_summary_test
fi
