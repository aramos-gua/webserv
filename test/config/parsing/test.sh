#!/usr/bin/env bash

source ../../../test/test-helpers.sh

# Check that parsing an invalid config file produces the expected error
run_error_test() {
	if [ "$CONFIG_FILE" ]; then
		CONFIG_FILE="../../../test/config/parsing/config-files/$CONFIG_FILE"
	fi
	export ACTUAL="$(../../../bin/config-parsing $CONFIG_FILE 2>&1)"
	cmp -s <(echo "$EXPECTED") <(echo "$ACTUAL") \
		&& (echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME") \
		|| (echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "$ACTUAL")
}

echo "*** ERROR TESTS ***"
echo -n "Run test suite? [Y/n] "
read -n 1 ANSWER
echo

if [ "$ANSWER" != "n" ]; then
	export TEST_NAME="Missing config file"
	export CONFIG_FILE=""
	export EXPECTED="\
Usage: ../../../bin/config-parsing <path to config file>"
	run_error_test

	export TEST_NAME="Bad config file: nonexistent"
	export CONFIG_FILE="nonexistent.conf"
	export EXPECTED="\
An error occurred during execution:
Couldn't open config file \"../../../test/config/parsing/config-files/nonexistent.conf\" for reading.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: missing 'events' block"
	export CONFIG_FILE="missing-events-block.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/missing-events-block.conf\": \"events\" block is required.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: missing 'http' block"
	export CONFIG_FILE="missing-http-block.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/missing-http-block.conf\": \"http\" block is required.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: missing 'server' block in 'http' context"
	export CONFIG_FILE="missing-server-block.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/missing-server-block.conf\": \"server\" block is required in \"http\" context.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: stray '}' at top level"
	export CONFIG_FILE="stray-closing-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/stray-closing-brace.conf\", line 1: Unexpected '}'.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: unterminated single quote"
	export CONFIG_FILE="unterminated-single-quote.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/unterminated-single-quote.conf\", line 2: Expected ';' after \"root\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: unterminated double quote"
	export CONFIG_FILE="unterminated-double-quote.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/unterminated-double-quote.conf\", line 2: Expected ';' after \"root\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: escape at end of file"
	export CONFIG_FILE="escape-at-end-of-file.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/escape-at-end-of-file.conf\", line 1: Expected ';' after \"root\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: bare ';' at directive position"
	export CONFIG_FILE="bare-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/bare-semicolon.conf\", line 1: Unexpected ';'.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: bare '{' at directive position"
	export CONFIG_FILE="bare-opening-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/bare-opening-brace.conf\", line 1: Unexpected '{'.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: unknown directive"
	export CONFIG_FILE="unknown-directive.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/unknown-directive.conf\", line 1: Unknown directive \"foo\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: duplicate directive"
	export CONFIG_FILE="duplicate-directive.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/duplicate-directive.conf\", line 2: Duplicate directive \"daemon\" in \"main\" context.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: directive used too deep: 'listen' in main context"
	export CONFIG_FILE="directive-too-deep.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/directive-too-deep.conf\", line 1: Directive \"listen\" not allowed in \"main\" context.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: directive used too shallow: 'worker_processes' in http context"
	export CONFIG_FILE="directive-too-shallow.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/directive-too-shallow.conf\", line 2: Directive \"worker_processes\" not allowed in \"http\" context.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'access_log' with missing value"
	export CONFIG_FILE="access-log-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/access-log-missing-value.conf\", line 2: Expected value for \"access_log\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'access_log' with too many arguments"
	export CONFIG_FILE="access-log-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/access-log-too-many-arguments.conf\", line 2: Too many arguments for \"access_log\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'access_log' with missing semicolon"
	export CONFIG_FILE="access-log-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/access-log-missing-semicolon.conf\", line 3: Expected ';' after \"access_log\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'alias' with missing value"
	export CONFIG_FILE="alias-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/alias-missing-value.conf\", line 4: Expected value for \"alias\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'alias' with too many arguments"
	export CONFIG_FILE="alias-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/alias-too-many-arguments.conf\", line 4: Too many arguments for \"alias\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'alias' with missing semicolon"
	export CONFIG_FILE="alias-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/alias-missing-semicolon.conf\", line 5: Expected ';' after \"alias\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'allow' with missing value"
	export CONFIG_FILE="allow-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/allow-missing-value.conf\", line 2: Expected value for \"allow\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'allow' with invalid IP address"
	export CONFIG_FILE="allow-invalid-ip-address.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/allow-invalid-ip-address.conf\", line 2: Invalid IP address or mask in access rule: not.an.ip.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: duplicate 'allow' for the same subnet"
	export CONFIG_FILE="duplicate-allow.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/duplicate-allow.conf\", line 4: Directive \"allow\" with value \"10.0.0.0/8\" conflicts with existing directive in \"server\" context.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'allow' and 'deny' for the same subnet"
	export CONFIG_FILE="conflicting-access-rules.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/conflicting-access-rules.conf\", line 4: Directive \"deny\" with value \"10.0.0.0/8\" conflicts with existing directive in \"server\" context.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'allow' with too many arguments"
	export CONFIG_FILE="allow-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/allow-too-many-arguments.conf\", line 2: Too many arguments for \"allow\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'allow' with missing semicolon"
	export CONFIG_FILE="allow-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/allow-missing-semicolon.conf\", line 3: Expected ';' after \"allow\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'autoindex' with missing value"
	export CONFIG_FILE="autoindex-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/autoindex-missing-value.conf\", line 2: Expected value for \"autoindex\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'autoindex' with invalid value"
	export CONFIG_FILE="autoindex-invalid-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/autoindex-invalid-value.conf\", line 2: Invalid value \"yes\" for \"autoindex\": expected 'on' or 'off'.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'autoindex' with too many arguments"
	export CONFIG_FILE="autoindex-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/autoindex-too-many-arguments.conf\", line 2: Too many arguments for \"autoindex\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'autoindex' with missing semicolon"
	export CONFIG_FILE="autoindex-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/autoindex-missing-semicolon.conf\", line 3: Expected ';' after \"autoindex\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'client_max_body_size' with missing value"
	export CONFIG_FILE="client-max-body-size-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/client-max-body-size-missing-value.conf\", line 2: Expected value for \"client_max_body_size\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'client_max_body_size' with non-numeric value"
	export CONFIG_FILE="client-max-body-size-non-numeric-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/client-max-body-size-non-numeric-value.conf\", line 2: Invalid number \"abc\" for \"client_max_body_size\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'client_max_body_size' with out-of-range number"
	export CONFIG_FILE="client-max-body-size-out-of-range-number.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/client-max-body-size-out-of-range-number.conf\", line 2: Number \"99999999999999999999\" out of range for \"client_max_body_size\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'client_max_body_size' with too many arguments"
	export CONFIG_FILE="client-max-body-size-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/client-max-body-size-too-many-arguments.conf\", line 2: Too many arguments for \"client_max_body_size\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'client_max_body_size' with missing semicolon"
	export CONFIG_FILE="client-max-body-size-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/client-max-body-size-missing-semicolon.conf\", line 3: Expected ';' after \"client_max_body_size\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'daemon' with missing value"
	export CONFIG_FILE="daemon-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/daemon-missing-value.conf\", line 1: Expected value for \"daemon\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'daemon' with invalid value"
	export CONFIG_FILE="daemon-invalid-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/daemon-invalid-value.conf\", line 1: Invalid value \"yes\" for \"daemon\": expected 'on' or 'off'.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'daemon' with too many arguments"
	export CONFIG_FILE="daemon-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/daemon-too-many-arguments.conf\", line 1: Too many arguments for \"daemon\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'daemon' with missing semicolon"
	export CONFIG_FILE="daemon-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/daemon-missing-semicolon.conf\", line 2: Expected ';' after \"daemon\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'default_type' with missing value"
	export CONFIG_FILE="default-type-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/default-type-missing-value.conf\", line 2: Expected value for \"default_type\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'default_type' with too many arguments"
	export CONFIG_FILE="default-type-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/default-type-too-many-arguments.conf\", line 2: Too many arguments for \"default_type\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'default_type' with missing semicolon"
	export CONFIG_FILE="default-type-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/default-type-missing-semicolon.conf\", line 3: Expected ';' after \"default_type\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'deny' with missing value"
	export CONFIG_FILE="deny-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/deny-missing-value.conf\", line 2: Expected value for \"deny\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'deny' with invalid IP address"
	export CONFIG_FILE="deny-invalid-ip-address.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/deny-invalid-ip-address.conf\", line 2: Invalid IP address or mask in access rule: not.an.ip.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'deny' with too many arguments"
	export CONFIG_FILE="deny-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/deny-too-many-arguments.conf\", line 2: Too many arguments for \"deny\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'deny' with missing semicolon"
	export CONFIG_FILE="deny-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/deny-missing-semicolon.conf\", line 3: Expected ';' after \"deny\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_log' with missing value"
	export CONFIG_FILE="error-log-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-log-missing-value.conf\", line 1: Expected value for \"error_log\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_log' with too many arguments"
	export CONFIG_FILE="error-log-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-log-too-many-arguments.conf\", line 1: Too many arguments for \"error_log\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_log' with missing semicolon"
	export CONFIG_FILE="error-log-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-log-missing-semicolon.conf\", line 2: Expected ';' after \"error_log\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_log' with unknown log level"
	export CONFIG_FILE="error-log-unknown-level.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-log-unknown-level.conf\", line 3: Unknown log level \"verbose\" for \"error_log\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_page' with missing value"
	export CONFIG_FILE="error-page-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-page-missing-value.conf\", line 2: Expected value for \"error_page\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_page' with only one argument"
	export CONFIG_FILE="error-page-only-one-argument.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-page-only-one-argument.conf\", line 2: \"error_page\" requires at least a status code and a URI.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_page' with response code modifier but no status codes"
	export CONFIG_FILE="error-page-response-code-modifier-but-no-status-codes.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-page-response-code-modifier-but-no-status-codes.conf\", line 2: \"error_page\" requires at least one status code.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_page' with non-numeric status code"
	export CONFIG_FILE="error-page-non-numeric-status-code.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-page-non-numeric-status-code.conf\", line 2: Invalid number \"foo\" for \"error_page\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_page' with out-of-range status code"
	export CONFIG_FILE="error-page-out-of-range-status-code.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-page-out-of-range-status-code.conf\", line 2: Invalid status code \"999\" for \"error_page\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_page' with invalid response code modifier"
	export CONFIG_FILE="error-page-invalid-response-code-modifier.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-page-invalid-response-code-modifier.conf\", line 2: Invalid number \"abc\" for \"error_page\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'error_page' with missing semicolon"
	export CONFIG_FILE="error-page-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/error-page-missing-semicolon.conf\", line 3: Expected ';' after \"error_page\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'events' with missing opening brace"
	export CONFIG_FILE="events-missing-opening-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/events-missing-opening-brace.conf\", line 1: Expected '{' for \"events\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'events' block with missing closing brace"
	export CONFIG_FILE="events-missing-closing-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/events-missing-closing-brace.conf\", line 3: Expected '}' for \"events\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'http' with missing opening brace"
	export CONFIG_FILE="http-missing-opening-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/http-missing-opening-brace.conf\", line 1: Expected '{' for \"http\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'http' block with missing closing brace"
	export CONFIG_FILE="http-missing-closing-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/http-missing-closing-brace.conf\", line 5: Expected '}' for \"http\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'include' with missing value"
	export CONFIG_FILE="include-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/include-missing-value.conf\", line 1: Expected value for \"include\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'include' with nonexistent file"
	export CONFIG_FILE="include-nonexistent-file.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/include-nonexistent-file.conf\", line 1: Couldn't open config file \"nonexistent-included-file.conf\" for reading.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'include' with too many arguments"
	export CONFIG_FILE="include-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/include-too-many-arguments.conf\", line 1: Too many arguments for \"include\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'include' with missing semicolon"
	export CONFIG_FILE="include-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/include-missing-semicolon.conf\", line 2: Expected ';' after \"include\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'include' with circular reference"
	export CONFIG_FILE="include-circular.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/include-circular.conf\", line 1: Circular include detected for \"include-circular.conf\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'index' with missing value"
	export CONFIG_FILE="index-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/index-missing-value.conf\", line 2: Expected value for \"index\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'index' with missing semicolon"
	export CONFIG_FILE="index-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/index-missing-semicolon.conf\", line 3: Expected ';' after \"index\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'internal' with too many arguments"
	export CONFIG_FILE="internal-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/internal-too-many-arguments.conf\", line 4: Too many arguments for \"internal\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'internal' with missing semicolon"
	export CONFIG_FILE="internal-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/internal-missing-semicolon.conf\", line 5: Expected ';' after \"internal\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'limit_except' with missing value"
	export CONFIG_FILE="limit-except-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/limit-except-missing-value.conf\", line 4: Expected value for \"limit_except\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'limit_except' with unknown HTTP method"
	export CONFIG_FILE="limit-except-unknown-http-method.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/limit-except-unknown-http-method.conf\", line 4: Unknown HTTP method \"FOO\" for \"limit_except\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'limit_except' with missing opening brace"
	export CONFIG_FILE="limit-except-missing-opening-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/limit-except-missing-opening-brace.conf\", line 4: Expected '{' for \"limit_except\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'limit_except' block with missing closing brace"
	export CONFIG_FILE="limit-except-missing-closing-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/limit-except-missing-closing-brace.conf\", line 6: Expected '}' for \"limit_except\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'listen' with missing value"
	export CONFIG_FILE="listen-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/listen-missing-value.conf\", line 3: Expected value for \"listen\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'listen' with invalid address"
	export CONFIG_FILE="listen-invalid-address.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/listen-invalid-address.conf\", line 3: Invalid value for \"listen\": Invalid port \"999999\": not in range 0 to 65535.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'listen' with unknown option"
	export CONFIG_FILE="listen-unknown-option.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/listen-unknown-option.conf\", line 3: Unknown option \"ssl\" for \"listen\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'listen' with missing semicolon"
	export CONFIG_FILE="listen-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/listen-missing-semicolon.conf\", line 4: Expected ';' after \"listen\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'location' with missing path after modifier"
	export CONFIG_FILE="location-missing-path-after-modifier.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/location-missing-path-after-modifier.conf\", line 3: Expected value for \"location\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'location' with missing opening brace"
	export CONFIG_FILE="location-missing-opening-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/location-missing-opening-brace.conf\", line 3: Expected '{' for \"location\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'location' with missing path after priority prefix modifier"
	export CONFIG_FILE="location-missing-path-after-priority-prefix-modifier.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/location-missing-path-after-priority-prefix-modifier.conf\", line 3: Expected value for \"location\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'location' prefix declared both with and without '^~'"
	export CONFIG_FILE="location-duplicate-prefix-modifiers.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/location-duplicate-prefix-modifiers.conf\", line 5: Directive \"location\" with value \"/assets/\" conflicts with existing directive in \"server\" context.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'location' block with missing closing brace"
	export CONFIG_FILE="location-missing-closing-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/location-missing-closing-brace.conf\", line 5: Expected '}' for \"location\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with missing value"
	export CONFIG_FILE="return-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-missing-value.conf\", line 3: Expected value for \"return\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with non-numeric status code"
	export CONFIG_FILE="return-non-numeric-status-code.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-non-numeric-status-code.conf\", line 3: Too many arguments for \"return\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with out-of-range status code"
	export CONFIG_FILE="return-out-of-range-status-code.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-out-of-range-status-code.conf\", line 3: Invalid status code \"999\" for \"return\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with too many arguments"
	export CONFIG_FILE="return-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-too-many-arguments.conf\", line 3: Too many arguments for \"return\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with missing semicolon"
	export CONFIG_FILE="return-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-missing-semicolon.conf\", line 4: Expected ';' after \"return\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with an informational status code"
	export CONFIG_FILE="return-informational-status-code.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-informational-status-code.conf\", line 3: 100 is an informational status code, which cannot be returned.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with an informational status code and a value"
	export CONFIG_FILE="return-informational-status-code-with-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-informational-status-code-with-value.conf\", line 3: 100 is an informational status code, which cannot be returned.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with a value but a status code that takes none"
	export CONFIG_FILE="return-value-with-no-content-status-code.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-value-with-no-content-status-code.conf\", line 3: 204 return response cannot have a value.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with a value but a status code that has no location"
	export CONFIG_FILE="return-value-with-not-modified-status-code.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-value-with-not-modified-status-code.conf\", line 3: 304 return response cannot have a value.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'return' with no body but a status code that needs one"
	export CONFIG_FILE="return-empty-with-success-status-code.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/return-empty-with-success-status-code.conf\", line 3: 200 return response cannot have response type EMPTY.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'root' with missing value"
	export CONFIG_FILE="root-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/root-missing-value.conf\", line 2: Expected value for \"root\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'root' with too many arguments"
	export CONFIG_FILE="root-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/root-too-many-arguments.conf\", line 2: Too many arguments for \"root\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'root' with missing semicolon"
	export CONFIG_FILE="root-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/root-missing-semicolon.conf\", line 3: Expected ';' after \"root\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'server' with missing opening brace"
	export CONFIG_FILE="server-missing-opening-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/server-missing-opening-brace.conf\", line 2: Expected '{' for \"server\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'server' block with missing closing brace"
	export CONFIG_FILE="server-missing-closing-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/server-missing-closing-brace.conf\", line 4: Expected '}' for \"server\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'server_name' with missing value"
	export CONFIG_FILE="server-name-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/server-name-missing-value.conf\", line 3: Expected value for \"server_name\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'server_name' with missing semicolon"
	export CONFIG_FILE="server-name-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/server-name-missing-semicolon.conf\", line 4: Expected ';' after \"server_name\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'try_files' with missing value"
	export CONFIG_FILE="try-files-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/try-files-missing-value.conf\", line 3: Expected value for \"try_files\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'try_files' with only one argument"
	export CONFIG_FILE="try-files-only-one-argument.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/try-files-only-one-argument.conf\", line 3: \"try_files\" requires at least one file and a fallback.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'try_files' with invalid status code fallback"
	export CONFIG_FILE="try-files-invalid-status-code-fallback.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/try-files-invalid-status-code-fallback.conf\", line 3: Invalid number \"abc\" for \"try_files\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'try_files' with missing semicolon"
	export CONFIG_FILE="try-files-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/try-files-missing-semicolon.conf\", line 4: Expected ';' after \"try_files\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'types' with missing opening brace"
	export CONFIG_FILE="types-missing-opening-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/types-missing-opening-brace.conf\", line 2: Expected '{' for \"types\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'types' block with missing closing brace"
	export CONFIG_FILE="types-missing-closing-brace.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/types-missing-closing-brace.conf\", line 4: Expected '}' for \"types\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: unexpected token inside 'types' block"
	export CONFIG_FILE="unexpected-token-inside-types-block.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/unexpected-token-inside-types-block.conf\", line 3: Unexpected token in \"types\" block.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: duplicate extension in 'types' block"
	export CONFIG_FILE="duplicate-extension-in-types-block.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/duplicate-extension-in-types-block.conf\", line 4: Duplicate extension \"html\" in \"types\" block.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'types' entry with missing semicolon"
	export CONFIG_FILE="types-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/types-missing-semicolon.conf\", line 4: Expected ';' after \"types\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'types' entry with missing extension"
	export CONFIG_FILE="types-missing-extension.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/types-missing-extension.conf\", line 3: Expected value for \"types\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'user' with missing value"
	export CONFIG_FILE="user-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/user-missing-value.conf\", line 1: Expected value for \"user\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'user' with too many arguments"
	export CONFIG_FILE="user-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/user-too-many-arguments.conf\", line 1: Too many values for \"user\" (expected at most 2).
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'user' with missing semicolon"
	export CONFIG_FILE="user-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/user-missing-semicolon.conf\", line 2: Expected ';' after \"user\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_connections' with missing value"
	export CONFIG_FILE="worker-connections-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-connections-missing-value.conf\", line 2: Expected value for \"worker_connections\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_connections' with non-numeric value"
	export CONFIG_FILE="worker-connections-non-numeric-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-connections-non-numeric-value.conf\", line 2: Invalid number \"abc\" for \"worker_connections\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_connections' with out-of-range number"
	export CONFIG_FILE="worker-connections-out-of-range-number.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-connections-out-of-range-number.conf\", line 2: Number \"99999999999999999999\" out of range for \"worker_connections\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_connections' with too many arguments"
	export CONFIG_FILE="worker-connections-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-connections-too-many-arguments.conf\", line 2: Too many arguments for \"worker_connections\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_connections' with missing semicolon"
	export CONFIG_FILE="worker-connections-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-connections-missing-semicolon.conf\", line 3: Expected ';' after \"worker_connections\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_processes' with missing value"
	export CONFIG_FILE="worker-processes-missing-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-processes-missing-value.conf\", line 1: Expected value for \"worker_processes\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_processes' with non-numeric value"
	export CONFIG_FILE="worker-processes-non-numeric-value.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-processes-non-numeric-value.conf\", line 1: Invalid number \"abc\" for \"worker_processes\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_processes' with out-of-range number"
	export CONFIG_FILE="worker-processes-out-of-range-number.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-processes-out-of-range-number.conf\", line 1: Number \"99999999999999999999\" out of range for \"worker_processes\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_processes' value greater than 1"
	export CONFIG_FILE="worker-processes-value-greater-than-1.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-processes-value-greater-than-1.conf\", line 1: Multiple worker processes are not currently supported (\"worker_processes\").
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_processes' with too many arguments"
	export CONFIG_FILE="worker-processes-too-many-arguments.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-processes-too-many-arguments.conf\", line 1: Too many arguments for \"worker_processes\".
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: 'worker_processes' with missing semicolon"
	export CONFIG_FILE="worker-processes-missing-semicolon.conf"
	export EXPECTED="An error occurred during execution:
\"../../../test/config/parsing/config-files/worker-processes-missing-semicolon.conf\", line 2: Expected ';' after \"worker_processes\".
Unable to continue."
	run_error_test
fi

# Check that parsing a valid config file produces the expected output
run_success_test() {
	CONFIG_FILE="../../../test/config/parsing/config-files/$CONFIG_FILE"
	export ACTUAL="$(../../../bin/config-parsing $CONFIG_FILE 2>&1)"
	cmp -s <(echo "$EXPECTED") <(echo "$ACTUAL") \
		&& (echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME") \
		|| (echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "$ACTUAL")
}

# Check that parsing two valid config files describing the same setup
# produces the same output, even if they look superficially different
# (e.g. their directives are in a different order)
run_equivalence_test() {
	FIRST_CONFIG_FILE=""
	MISMATCHED_CONFIG_FILE=""
	export EXPECTED=""
	export ACTUAL=""
	for EQUIVALENT_CONFIG_FILE in $CONFIG_FILES; do
		OUTPUT="$(../../../bin/config-parsing \
			"../../../test/config/parsing/config-files/$EQUIVALENT_CONFIG_FILE" \
			2>&1)"
		if [ -z "$FIRST_CONFIG_FILE" ]; then
			FIRST_CONFIG_FILE="$EQUIVALENT_CONFIG_FILE"
			EXPECTED="$OUTPUT"
		elif [ "$OUTPUT" != "$EXPECTED" ] \
			&& [ -z "$MISMATCHED_CONFIG_FILE" ]; then
			MISMATCHED_CONFIG_FILE="$EQUIVALENT_CONFIG_FILE"
			ACTUAL="$OUTPUT"
		fi
	done
	[ -z "$MISMATCHED_CONFIG_FILE" ] \
		&& (echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME") \
		|| (echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "$MISMATCHED_CONFIG_FILE differs from $FIRST_CONFIG_FILE:" \
			&& diff <(echo "$EXPECTED") <(echo "$ACTUAL"))
}

# Check that parsing a valid config file, printing and parsing the result
# produces the same output (i.e. making sure printing doesn't lose information)
run_round_trip_test() {
	CONFIG_FILE="../../../test/config/parsing/config-files/$CONFIG_FILE"
	export EXPECTED="$(../../../bin/config-parsing $CONFIG_FILE 2>&1)"
	export ACTUAL="$(echo "$EXPECTED" \
		| ../../../bin/config-parsing /dev/stdin 2>&1)"
	cmp -s <(echo "$EXPECTED") <(echo "$ACTUAL") \
		&& (echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME") \
		|| (echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& diff <(echo "$EXPECTED") <(echo "$ACTUAL"))
}

echo "*** SUCCESS TESTS ***"
echo -n "Run test suite? [Y/n] "
read -n 1 ANSWER
echo

if [ "$ANSWER" != "n" ]; then
	export TEST_NAME="Minimal config file"
	export CONFIG_FILE="minimal.conf"
	export EXPECTED="\
events {
    worker_connections 1024;
}
http {
    server {
        listen 0.0.0.0:80 default_server;
    }
}"
	run_success_test

	export TEST_NAME="Listen addresses, server names and server blocks normalised"
	export CONFIG_FILE="server-printing-normalisation.conf"
	export EXPECTED="\
events {
    worker_connections 1024;
}
http {
    server {
        listen 0.0.0.0:8080 default_server;
        listen 0.0.0.0:8082 default_server;
        server_name alpha.example.com zeta.example.com;
        root /srv/multi;
    }
    server {
        listen 0.0.0.0:8080;
        root /srv/catch-all;
    }
    server {
        listen 0.0.0.0:8082;
        server_name beta.example.com;
        root /srv/beta;
    }
}"
	run_success_test

	export TEST_NAME="Every directive, scrambled, with an included file"
	export CONFIG_FILE="all-directives.conf"
	export EXPECTED="\
daemon on;
error_log /var/log/penguinx/error.log error;
user www-data www-group;
worker_processes 1;
events {
    worker_connections 512;
}
http {
    access_log /var/log/penguinx/access.log;
    allow 0.0.0.0/0;
    allow ::/0;
    autoindex off;
    client_max_body_size 1048576;
    default_type application/octet-stream;
    error_page 404 /404.html;
    error_page 500 =200 /50x.html;
    error_page 502 =200 /50x.html;
    error_page 503 =200 /50x.html;
    error_page 504 =200 /50x.html;
    index index.html index.htm;
    root /srv/penguinx;
    types {
        application/json json;
        image/png png;
        text/css css;
        text/html htm html;
    }
    server {
        listen 0.0.0.0:8080 default_server;
        listen 127.0.0.1:9090 default_server;
        listen [::]:8080 default_server;
        server_name alpha.example.com zeta.example.com;
        error_log /var/log/penguinx/server-error.log;
        error_page 403 /403.html;
        root /srv/penguinx/main;
        try_files /index.html /index.htm =404;
        location = /cached {
            return 304;
        }
        location = /health {
            return 200 alive;
        }
        location ^~ /assets/img/ {
            root /srv/penguinx/images;
        }
        location ^~ /assets/ {
            autoindex on;
            root /srv/penguinx/static;
        }
        location /downloads/ {
            alias /srv/penguinx/files/;
            autoindex on;
            limit_except GET HEAD {
                access_log /var/log/penguinx/denied.log;
                deny 0.0.0.0/0;
                deny ::/0;
            }
        }
        location /private/ {
            deny 10.0.0.0/8;
            internal;
        }
        location ~\$ .php {
            return 502;
        }
    }
    server {
        listen 0.0.0.0:8080;
        listen [::]:8080;
        return 301 http://alpha.example.com/;
    }
}"
	run_success_test

	export TEST_NAME="Locations grouped by modifier, priority prefixes ahead of the rest"
	export CONFIG_FILE="location-priority-prefix.conf"
	export EXPECTED="\
events {
    worker_connections 1024;
}
http {
    server {
        listen 0.0.0.0:8080 default_server;
        listen [::]:8080 default_server;
        location = /assets/logo.png {
            return 304;
        }
        location ^~ /assets/fonts/ {
            root /srv/fonts;
            location ^~ /assets/fonts/vendor/ {
                internal;
            }
        }
        location ^~ /static/ {
            root /srv/static;
        }
        location /assets/ {
            autoindex on;
        }
        location ~\$ .php {
            return 502;
        }
    }
}"
	run_success_test

	export TEST_NAME="Every 'error_log' level, and the form that omits it"
	export CONFIG_FILE="error-log-levels.conf"
	export EXPECTED="\
events {
    worker_connections 1024;
}
http {
    error_log /var/log/penguinx/http.log;
    server {
        listen 0.0.0.0:8080 default_server;
        listen [::]:8080 default_server;
        error_log /var/log/penguinx/server.log warn;
        location = /debug {
            error_log /var/log/penguinx/debug.log debug;
        }
        location = /error {
            error_log /var/log/penguinx/error.log error;
        }
        location = /fatal {
            error_log /var/log/penguinx/fatal.log fatal;
        }
        location = /info {
            error_log /var/log/penguinx/info.log info;
        }
        location = /none {
            error_log /var/log/penguinx/none.log;
        }
    }
}"
	run_success_test

	export TEST_NAME="Same file included twice in sequence"
	export CONFIG_FILE="include-sequential.conf"
	export EXPECTED="\
events {
    worker_connections 1024;
}
http {
    server {
        listen 0.0.0.0:8080 default_server;
        listen [::]:8080 default_server;
        server_name first.example.com;
        autoindex on;
        root /srv/shared;
    }
    server {
        listen 0.0.0.0:8081 default_server;
        listen [::]:8081 default_server;
        server_name second.example.com;
        autoindex on;
        root /srv/shared;
    }
}"
	run_success_test

	export TEST_NAME="Implicit default server made explicit: first server declared"
	export CONFIG_FILE="implicit-default-server-aaa-first.conf"
	export EXPECTED="\
events {
    worker_connections 1024;
}
http {
    server {
        listen 0.0.0.0:8080 default_server;
        server_name aaa.example.com;
    }
    server {
        listen 0.0.0.0:8080;
        server_name mmm.example.com;
    }
    server {
        listen 0.0.0.0:8080;
        server_name zzz.example.com;
    }
}"
	run_success_test

	export TEST_NAME="Implicit default server made explicit: second server declared"
	export CONFIG_FILE="implicit-default-server-mmm-first.conf"
	export EXPECTED="\
events {
    worker_connections 1024;
}
http {
    server {
        listen 0.0.0.0:8080;
        server_name aaa.example.com;
    }
    server {
        listen 0.0.0.0:8080 default_server;
        server_name mmm.example.com;
    }
    server {
        listen 0.0.0.0:8080;
        server_name zzz.example.com;
    }
}"
	run_success_test

	export TEST_NAME="Implicit default server made explicit: third server declared"
	export CONFIG_FILE="implicit-default-server-zzz-first.conf"
	export EXPECTED="\
events {
    worker_connections 1024;
}
http {
    server {
        listen 0.0.0.0:8080;
        server_name aaa.example.com;
    }
    server {
        listen 0.0.0.0:8080;
        server_name mmm.example.com;
    }
    server {
        listen 0.0.0.0:8080 default_server;
        server_name zzz.example.com;
    }
}"
	run_success_test

	export TEST_NAME="Server order irrelevant across distinct address/port pairs"
	export CONFIG_FILES="\
server-order-ascending.conf \
server-order-descending.conf \
server-order-shuffled.conf"
	run_equivalence_test

	export TEST_NAME="Server order irrelevant on a shared address/port pair"
	export CONFIG_FILES="\
shared-port-explicit-default-named-first.conf \
shared-port-explicit-default-default-first.conf \
shared-port-explicit-default-catch-all-first.conf"
	run_equivalence_test

	export TEST_NAME="Directive order and normalisable spellings irrelevant"
	export CONFIG_FILES="\
all-directives.conf \
all-directives-reordered.conf"
	run_equivalence_test

	export TEST_NAME="Printed config reparses unchanged: minimal"
	export CONFIG_FILE="minimal.conf"
	run_round_trip_test

	export TEST_NAME="Printed config reparses unchanged: normalised servers"
	export CONFIG_FILE="server-printing-normalisation.conf"
	run_round_trip_test

	export TEST_NAME="Printed config reparses unchanged: sequential includes"
	export CONFIG_FILE="include-sequential.conf"
	run_round_trip_test

	export TEST_NAME="Printed config reparses unchanged: error log levels"
	export CONFIG_FILE="error-log-levels.conf"
	run_round_trip_test

	export TEST_NAME="Printed config reparses unchanged: priority prefix locations"
	export CONFIG_FILE="location-priority-prefix.conf"
	run_round_trip_test

	export TEST_NAME="Printed config reparses unchanged: every directive"
	export CONFIG_FILE="all-directives.conf"
	run_round_trip_test

	export TEST_NAME="Printed config reparses unchanged: implicit default server"
	export CONFIG_FILE="implicit-default-server-zzz-first.conf"
	run_round_trip_test

	export TEST_NAME="Printed config reparses unchanged: catch-all declared first"
	export CONFIG_FILE="shared-port-explicit-default-catch-all-first.conf"
	run_round_trip_test
fi
